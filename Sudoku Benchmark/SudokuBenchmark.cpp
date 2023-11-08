#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "./SudokuSolver.cpp"
#include <fstream>
#include <chrono>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
    size_t total_size = size * nmemb;
    output->append(static_cast<char *>(contents), total_size);
    return total_size;
}

int main()
{
    std::string csv_filename = "dados.csv";

    // Abrindo o arquivo CSV para gravação
    std::ofstream csv_file(csv_filename);
    if (csv_file.is_open())
    {
        for (int n = 0; n < 5; n++)
        {
            CURL *curl;
            CURLcode res;
            std::string api_url = "https://sudoku-api.vercel.app/api/dosuku"; // Substitua pela URL da sua API

            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();

            if (curl)
            {
                curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

                std::string response_data;
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

                res = curl_easy_perform(curl);

                if (res == CURLE_OK)
                {
                    nlohmann::json json_data = nlohmann::json::parse(response_data);

                    std::string difficulty = json_data["newboard"]["grids"][0]["difficulty"];
                    auto value = json_data["newboard"]["grids"][0]["value"];

                    int matrix[9][9];

                    // Preenchendo a matriz com os valores da matriz JSON
                    for (int i = 0; i < 9; i++)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            matrix[i][j] = value[i][j];
                        }
                    }

                    auto start = std::chrono::steady_clock::now();
                    bool solved = solve(matrix); 
                    auto end = std::chrono::steady_clock::now();

                    auto elapsed = end - start;
                    if (solved)
                    {
                        csv_file << difficulty << ";" << elapsed.count() << "ns\n";
                    }
                }
                else
                {
                    std::cerr << "Erro na solicitação HTTP: " << curl_easy_strerror(res) << std::endl;
                }

                curl_easy_cleanup(curl);
            }

            curl_global_cleanup();
        }
        std::cout << "Os dados foram gravados em '" << csv_filename << "'." << std::endl;
        csv_file.close();
    }
    else
    {
        std::cerr << "Erro ao abrir o arquivo CSV para gravação." << std::endl;
    }

    return 0;
}

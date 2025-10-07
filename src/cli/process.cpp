#include "process.hpp"
#include <algorithm>
#include <fstream>
#include <future>
#include <iostream>
#include <nlohmann/json.hpp>
#include <semaphore>
#include <sstream>
#include <string>
#include <cstdlib>

namespace Proc {
    ConciseModel::ConciseModel(
            std::string npi, std::string npiRegistryEnumerationType,
            std::string npiRegistryCredentials, std::string npiRegistrySex,
            std::string npiRegistryStatus, std::string npiRegistryFirstName,
            std::string npiRegistryLastName, std::string npiRegistryMiddleName,
            std::string npiRegistrySoleProprietor,
            std::string npiRegistryEnumerationDate,
            std::string npiRegistryLastUpdated,
            std::string npiRegistryCertificationDate,
            nlohmann::json npiRegistryTaxonomies,
            nlohmann::json npiRegistryAddresses,
            nlohmann::json practitionerModel, nlohmann::json organizationModel,
            nlohmann::json locationModel, std::string target)
        : npi_{npi}, npiRegistryEnumerationType_{npiRegistryEnumerationType},
          npiRegistryCredentials_{npiRegistryCredentials},
          npiRegistrySex_{npiRegistrySex}, npiRegistryStatus_{npiRegistryStatus},
          npiRegistryFirstName_{npiRegistryFirstName},
          npiRegistryLastName_{npiRegistryLastName},
          npiRegistryMiddleName_{npiRegistryMiddleName},
          npiRegistrySoleProprietor_{npiRegistrySoleProprietor},
          npiRegistryEnumerationDate_{npiRegistryEnumerationDate},
          npiRegistryLastUpdated_{npiRegistryLastUpdated},
          npiRegistryCertificationDate_{npiRegistryCertificationDate},
          npiRegistryTaxonomies_{npiRegistryTaxonomies},
          npiRegistryAddresses_{npiRegistryAddresses},
          practitionerModel_{practitionerModel},
          organizationModel_{organizationModel}, locationModel_{locationModel},
          target_{target}
    {}

    std::string ConciseModel::toString()
    {
        return std::format(
                "{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}\n",
                this->npi_, this->npiRegistryEnumerationType_,
                this->npiRegistryCredentials_, this->npiRegistrySex_,
                this->npiRegistryStatus_, this->npiRegistryFirstName_,
                this->npiRegistryLastName_, this->npiRegistryMiddleName_,
                this->npiRegistrySoleProprietor_,
                this->npiRegistryEnumerationDate_, this->npiRegistryLastUpdated_,
                this->npiRegistryCertificationDate_,
                this->npiRegistryTaxonomies_.dump(),
                this->npiRegistryAddresses_.dump(),
                this->practitionerModel_.dump(),
                this->organizationModel_.dump(),
                this->locationModel_.dump(), this->target_);
    }

    Cli::Cli(const std::int32_t argc, const std::vector<std::string>& argv)
    {
        this->argc_ = argc;
        this->argv_ = argv;
    }

    bool Cli::isValidFile(const std::string& path)
    {
        const std::ifstream file(path);
        return file.is_open();
    }

    std::int16_t Cli::getNpiIndex(const std::string& npiIndex)
    {
        std::string num{};
        for (auto& ch: npiIndex) {
            if (isdigit(ch)) { num.push_back(ch); }
        }
        return std::stoi(num);
    }

    std::map<std::string, std::vector<std::string>>
    Cli::readFile(std::string filePath, std::string delimiter,
                  std::string containsHeaders, std::string npiIndex)
    {
        std::ifstream file(filePath);
        std::vector<std::string> npiData;

        bool containsHeadersParsed = containsHeaders == "true";
        std::size_t npiIndexParsed = getNpiIndex(npiIndex);

        if (this->isValidFile(filePath) && filePath.ends_with(".csv")
            || this->isValidFile(filePath) && filePath.ends_with(".txt")) {
            std::string line;
            std::size_t rowNumber = 0;
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string token;
                std::vector<std::string> rowTokens;
                while (std::getline(ss, token, delimiter[0])) {
                    rowTokens.push_back(token);
                }
                if (containsHeadersParsed && rowNumber == 0) {
                    rowNumber++;
                    continue;
                }

                for (std::size_t i = 0; i < rowTokens.size(); i++) {
                    if (i == npiIndexParsed) {
                        npiData.push_back(rowTokens[i]);
                    }
                }
            }
            file.close();
        }
        auto result = std::map<std::string, std::vector<std::string>>();
        result["npi"] = npiData;
        return result;
    }

    std::string Cli::processData(std::string url, std::string npi,
                                 std::string model, std::string target)
    {
        cpr::Response response = Cli::requestData(url, npi, model, target);
        if (response.status_code == 200) {
            nlohmann::json jsonData = nlohmann::json::parse(response.text);
            if (jsonData["npiRegistryTaxonomies"][0].is_array()) {
                std::cout << jsonData["npiRegistryTaxonomies"][0].dump() << std::endl;
            }


            std::string npiValue{};
            std::string npiRegistryEnumerationType{};
            std::string npiRegistryCredentials{};
            std::string npiRegistrySex{};
            std::string npiRegistryStatus{};
            std::string npiRegistryFirstName{};
            std::string npiRegistryLastName{};
            std::string npiRegistryMiddleName{};
            std::string npiRegistrySoleProprietor{};
            std::string npiRegistryEnumerationDate{};
            std::string npiRegistryLastUpdated{};
            std::string npiRegistryCertificationDate{};
            nlohmann::json npiRegistryTaxonomies;
            nlohmann::json npiRegistryAddresses;
            nlohmann::json practitionerModel;
            nlohmann::json organizationModel;
            nlohmann::json locationModel;
            std::string targetTemp{};

            if (jsonData.contains("npi") && jsonData["npi"].is_string()) {
                npiValue = jsonData["npi"];
            }

            if (jsonData.contains("npiRegistryConciseModel")
                && jsonData["npiRegistryConciseModel"].contains(
                        "npiRegistryEnumerationType")
                && jsonData["npiRegistryConciseModel"]
                           ["npiRegistryEnumerationType"]
                                   .is_string()) {
                npiRegistryEnumerationType = jsonData["npiRegistryConciseModel"]
                                  ["npiRegistryEnumerationType"];
            }
            if (jsonData.contains("npiRegistryConciseModel")
                && jsonData["npiRegistryConciseModel"].contains(
                        "npiRegistryCredential")
                && jsonData["npiRegistryConciseModel"]["npiRegistryCredential"]
                           .is_string()) {
                npiRegistryCredentials = jsonData["npiRegistryConciseModel"]
                                                 ["npiRegistryCredential"];
            }
            if (jsonData.contains("npiRegistryConciseModel")
                && jsonData["npiRegistryConciseModel"].contains(
                        "npiRegistrySex")
                && jsonData["npiRegistryConciseModel"]["npiRegistrySex"]
                           .is_string()) {
                npiRegistrySex = jsonData["npiRegistryConciseModel"]["npiRegistrySex"];
            }
            if (jsonData.contains("npiRegistryConciseModel")
                && jsonData["npiRegistryConciseModel"].contains(
                        "npiRegistryStatus")
                && jsonData["npiRegistryConciseModel"]["npiRegistryStatus"]
                           .is_string()) {
                npiRegistryStatus = jsonData["npiRegistryConciseModel"]
                                            ["npiRegistryStatus"];
            }
            if (jsonData.contains("npiRegistryConciseModel")
                && jsonData["npiRegistryConciseModel"].contains(
                        "npiRegistryFirstName")
                && jsonData["npiRegistryConciseModel"]["npiRegistryFirstName"]
                           .is_string()) {
                npiRegistryFirstName = jsonData["npiRegistryConciseModel"]
                                               ["npiRegistryFirstName"];
            }
            if (jsonData.contains("npiRegistryConciseModel")
                && jsonData["npiRegistryConciseModel"].contains(
                        "npiRegistryLastName")
                && jsonData["npiRegistryConciseModel"]["npiRegistryLastName"]
                           .is_string()) {
                npiRegistryLastName = jsonData["npiRegistryConciseModel"]
                                              ["npiRegistryLastName"];
            }
            if (jsonData.contains("npiRegistryConciseModel")
                && jsonData["npiRegistryConciseModel"].contains(
                        "npiRegistryMiddleName")
                && jsonData["npiRegistryConciseModel"]["npiRegistryMiddleName"]
                           .is_string()) {
                npiRegistryMiddleName = jsonData["npiRegistryConciseModel"]
                                                ["npiRegistryMiddleName"];
            }

            if (jsonData.contains("npiRegistryConciseModel")
                && jsonData["npiRegistryConciseModel"].contains(
                        "npiRegistrySoleProprietor")
                && jsonData["npiRegistryConciseModel"]
                           ["npiRegistrySoleProprietor"]
                                   .is_string()) {
                npiRegistrySoleProprietor = jsonData["npiRegistryConciseModel"]
                                  ["npiRegistrySoleProprietor"];
            }

            if (jsonData.contains("npiRegistryConciseModel")
                && jsonData["npiRegistryConciseModel"].contains(
                        "npiRegistryEnumerationDate")
                && jsonData["npiRegistryConciseModel"]
                           ["npiRegistryEnumerationDate"]
                                   .is_string()) {
                npiRegistryEnumerationDate = jsonData["npiRegistryConciseModel"]
                                  ["npiRegistryEnumerationDate"];
            }

            if (jsonData.contains("npiRegistryConciseModel")
                && jsonData["npiRegistryConciseModel"].contains(
                        "npiRegistryLastUpdated")
                && jsonData["npiRegistryConciseModel"]["npiRegistryLastUpdated"]
                           .is_string()) {
                npiRegistryLastUpdated = jsonData["npiRegistryConciseModel"]
                                                 ["npiRegistryLastUpdated"];
            }

            if (jsonData.contains("npiRegistryConciseModel")
                && jsonData["npiRegistryConciseModel"].contains(
                        "npiRegistryCertificationDate")
                && jsonData["npiRegistryConciseModel"]
                           ["npiRegistryCertificationDate"]
                                   .is_string()) {
                npiRegistryCertificationDate = jsonData["npiRegistryConciseModel"]
                                  ["npiRegistryCertificationDate"];
            }

            if (jsonData.contains("npiRegistryConciseModel")
                && jsonData["npiRegistryConciseModel"].contains(
                        "npiRegistryTaxonomies")) {
                npiRegistryTaxonomies = jsonData["npiRegistryConciseModel"]
                                                ["npiRegistryTaxonomies"];
            }

            if (jsonData.contains("npiRegistryConciseModel")
                && jsonData["npiRegistryConciseModel"].contains(
                        "npiRegistryAddresses")) {
                npiRegistryAddresses = jsonData["npiRegistryConciseModel"]
                                               ["npiRegistryAddresses"];
            }

            if (jsonData.contains("practitionerModel")) {
                practitionerModel = jsonData["practitionerModel"];
            }

            if (jsonData.contains("organizationModel")) {
                organizationModel = jsonData["organizationModel"];
            }

            if (jsonData.contains("locationModel")) {
                locationModel = jsonData["locationModel"];
            }

            if (jsonData.contains("target")) {
                targetTemp = jsonData["target"];
            }

            auto result = ConciseModel(
                    npiValue, npiRegistryEnumerationType, npiRegistryCredentials,
                    npiRegistrySex, npiRegistryStatus, npiRegistryFirstName,
                    npiRegistryLastName, npiRegistryMiddleName,
                    npiRegistrySoleProprietor, npiRegistryEnumerationDate,
                    npiRegistryLastUpdated, npiRegistryCertificationDate,
                    npiRegistryTaxonomies, npiRegistryAddresses,
                    practitionerModel, organizationModel, locationModel,
                    targetTemp);
            return result.toString();
        }
        return std::format("INVALID;STATUS_CODE;ISSUE;---> npi:{},target:{},status_code:{}", npi, target, response.status_code);
    }

    cpr::Response Cli::requestData(std::string url, std::string npi,
                                   std::string model, std::string target)
    {
        cpr::Response r = cpr::Get(
            cpr::Url{url},
            cpr::Parameters{{"npi", npi},
            {"model", model},
         {"target", target}}
        );
        return r;
    }

    std::int32_t Cli::getArgc()
    {
        return this->argc_;
    }

    std::vector<std::string> Cli::getArgs()
    {
        return this->argv_;
    }

    std::map<std::string, std::string> Cli::parseArgs()
    {
        std::map<std::string, std::string> mappedArgs{};
        const auto arguments = this->getArgs();
        for (std::size_t i = 1; i < this->getArgc(); i++) {
            if (arguments[i].find("--filePath=") != std::string::npos) {
                const auto filePath = arguments[i].substr(
                        arguments[i].find("=") + 1,
                        arguments[i].length() - arguments[i].find("=") - 1);
                mappedArgs["filePath"] = filePath;
            }
            else if (arguments[i].compare("--del=comma") == 0) {
                mappedArgs["inputFileDelimiter"] = ",";
            }
            else if (arguments[i].compare("--headers=true") == 0) {
                mappedArgs["inputFileHeaders"] = "true";
            }
            else if (arguments[i].compare("--headers=false") == 0) {
                mappedArgs["inputFileHeaders"] = "false";
            }
            else if (arguments[i].find("--npiColumn=") != std::string::npos) {
                auto column = arguments[i].substr(
                        arguments[i].find("=") + 1,
                        arguments[i].length() - arguments[i].find("=") - 1);
                mappedArgs["npiColumnIndex"] = column;
            }
            else if (arguments[i].find("--apiUrl=") != std::string::npos) {
                auto url = arguments[i].substr(
                        arguments[i].find("=") + 1,
                        arguments[i].length() - arguments[i].find("=") - 1);
                mappedArgs["apiUrl"] = url;
            }
            else if (arguments[i].find("--target=") != std::string::npos) {
                auto targets = arguments[i].substr(
                        arguments[i].find("=") + 1,
                        arguments[i].length() - arguments[i].find("=") - 1);
                mappedArgs["targets"] = targets;
            } else {
                std::cout << "=====================================" << "\n";
                std::cout << "Please Provide All Necessary Args:" << "\n";
                std::cout << "--filePath=/example/file.txt" << "\n";
                std::cout << "--del=comma (only comma delimited supported now)" << "\n";
                std::cout << "--headers=true OR --headers=false" << "\n";
                std::cout << "--npiColumn=7 (index of column Ex: 0 or 1 or 2)" << "\n";
                std::cout << "--apiUrl=https://example-url/example/" << "\n";
                std::cout << "--target=TARGET1,TARGET2,TARGET3 (targets are comma delimited array)" << "\n";
                std::cout << "=====================================" << "\n";
                exit(EXIT_SUCCESS);
            }
        }
        return mappedArgs;
    }

    std::vector<std::string> Cli::splitTargets(const std::string& targets)
    {
        std::vector<std::string> splitTargets;
        std::stringstream ss(targets);
        std::string item;

        while (std::getline(ss, item, ',')) { splitTargets.push_back(item); }
        return splitTargets;
    }

    void Cli::init()
    {
        auto mappedArgs = parseArgs();

        if (!this->isValidFile(mappedArgs["filePath"])) {
            std::cerr << "Error: " << mappedArgs["filePath"]
                      << " is not a valid file\n";
        }

        auto data = this->readFile(
                mappedArgs["filePath"], mappedArgs["inputFileDelimiter"],
                mappedArgs["inputFileHeaders"], mappedArgs["npiColumnIndex"]);

        auto splitTargets = this->splitTargets(mappedArgs["targets"]);

        std::vector<std::future<std::string>> futures;
        unsigned int maxNumThreads = std::thread::hardware_concurrency();
        constexpr int maxPermits = 4; // Implement throttling to respect NPI Registry API’s undocumented rate and usage limits
        std::counting_semaphore<maxPermits> sem(maxPermits); // max concurrent tasks
        std::cout << "Max # threads on host: " << maxNumThreads << "\n";
        std::cout << "Max # threads used for execution: " << maxPermits << "\n";
        for (auto& target: splitTargets) {
            std::string apiUrl = mappedArgs["apiUrl"];

            for (auto& npi: data["npi"]) {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                sem.acquire(); // wait if too many tasks are running
                npi.erase(std::remove(npi.begin(), npi.end(), '"'), npi.end());
                futures.push_back(std::async(
                        std::launch::async,
                        [&sem, npi = std::string(npi),
                         target = std::string(target),
                         apiUrl = std::string(apiUrl)]() -> std::string {
                            std::string processData = Cli::processData(
                                    apiUrl, npi, "CONCISE", target);
                            sem.release();
                            return processData;
                        }));
            }

            int i = 0;
            for (auto& future: futures) {
                auto result = future.get();
                if (result.find("INVALID;STATUS_CODE;ISSUE;--->") != std::string::npos) {
                    std::fstream errorFile(std::format("{}_error.txt", target),
                                           std::ios::app);
                    if (errorFile.is_open()) { errorFile << result << "\n"; }
                    errorFile.close();
                }
                else {
                    std::fstream outputFile(std::format("{}.txt", target),
                                            std::ios::app);

                    if (outputFile.is_open() && i == 0) {
                        outputFile << CONCISE_MODEL_COL_NAME << "\n";
                        outputFile << result;
                        i++;
                    }
                    else if (outputFile.is_open()) {
                        outputFile << result;
                    }
                    outputFile.close();
                }
            }
            futures.clear();
        }
    }

} // namespace Proc

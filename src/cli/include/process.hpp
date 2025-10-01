#ifndef FHIR_UNIFIER_CLIENT_DISPLAY_HPP
#define FHIR_UNIFIER_CLIENT_DISPLAY_HPP

#include <cstdint>
#include <map>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

namespace Proc {
    const std::string CONCISE_MODEL_COL_NAME = "npi|npi_registry_enumeration_type|npi_registry_credential|npi_registry_sex|npi_registry_status|npi_registry_first_name|npi_registry_last_name|npi_registry_middle_name|npi_registry_sole_proprietor|npi_registry_enumeration_date|npi_registry_last_updated|npi_registry_certification_date|npi_registry_taxonomies|npi_registry_addresses|practitioner_model|organization_model|location_model|target";
    class ConciseModel {
    private:
        std::string npi;
        std::string npiRegistryEnumerationType;
        std::string npiRegistryCredentials;
        std::string npiRegistrySex;
        std::string npiRegistryStatus;
        std::string npiRegistryFirstName;
        std::string npiRegistryLastName;
        std::string npiRegistryMiddleName;
        std::string npiRegistrySoleProprietor;
        std::string npiRegistryEnumerationDate;
        std::string npiRegistryLastUpdated;
        std::string npiRegistryCertificationDate;
        nlohmann::json npiRegistryTaxonomies;
        nlohmann::json npiRegistryAddresses;
        nlohmann::json practitionerModel;
        nlohmann::json organizationModel;
        nlohmann::json locationModel;
        std::string target;
    public:
        ConciseModel(
            std::string npi,
            std::string npiRegistryEnumerationType,
            std::string npiRegistryCredentials,
            std::string npiRegistrySex,
            std::string npiRegistryStatus,
            std::string npiRegistryFirstName,
            std::string npiRegistryLastName,
            std::string npiRegistryMiddleName,
            std::string npiRegistrySoleProprietor,
            std::string npiRegistryEnumerationDate,
            std::string npiRegistryLastUpdated,
            std::string npiRegistryCertificationDate,
            nlohmann::json npiRegistryTaxonomies,
            nlohmann::json npiRegistryAddresses,
            nlohmann::json practitionerModel,
            nlohmann::json organizationModel,
            nlohmann::json locationModel,
            std::string target
            );
        std::string toString();

    };
    class Cli {
        std::int32_t argc;
        std::vector<std::string> argv;
    public:
        Cli(std::int32_t argc, const std::vector<std::string>& argv);
        void init();
        static bool isValidFile(const std::string& path);
        static std::int16_t getNpiIndex(const std::string& npiIndex);
        std::map<std::string, std::vector<std::string>>
        readFile(std::string filePath, std::string delimiter,
            std::string containsHeaders, std::string npiHeaderIndex);

        static std::string processData(
            std::string url, std::string npi,
            std::string model, std::string target
        );
        static cpr::Response requestData(
            std::string url, std::string npi,
            std::string model, std::string target
        );
        std::map<std::string, std::string> parseArgs();
        std::vector<std::string> getArgs();
        std::int32_t getArgc();
        static std::vector<std::string> splitTargets(const std::string& targets);
    };

}

#endif

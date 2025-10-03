## Valid Arguments 🚀
- --filePath
  - The path to the input file.
- --del=comma
  - The delimiter of the input file.
- --headers
  - If input file contains headers.
- --npiColumn
  - Index of npi column in dataset.
- --apiUrl
  - Url to the api endpoint.
- --target
  - Comma seperated list/array of target.

## CLI Example 📖

```bash

fhir-unifier-client.exe 
      --filePath=./input_data.csv 
      --del=comma 
      --headers=true 
      --npiColumn=0 
      --apiUrl="api-endpoint" 
      --target=CIGNA,UNITED_HEALTH_CARE
```


# PDSC

<!-- [![version](https://img.shields.io/badge/cpp-c%2B%2B20-red)](https://github.com/intellistream/PDSC) -->
<!-- [![os](https://img.shields.io/badge/os-linux|macOS-yellow)](https://github.com/intellistream/PDSC) -->

## About

PDSC is a C++ stream clustering benchmark for educational purposes

## Build Sesame

### Prerequisites
- gcc 11 or lator
- cmake 3.17 or later

### Checkout Source Code
```bash
git clone https://github.com/intellistream/PDSC
cd PDSC
```

### Build
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### Run Benchmark
Download datasets from [Releases](https://github.com/intellistream/PDSC/releases/tag/dataset):
```bash
wget https://github.com/intellistream/PDSC/releases/download/dataset/covtype.csv
```
Run the benchmark:
```bash
./pdsc /path/to/{dataset}.csv [-n num_points]
```

## Datasets

| DataSet   | Length | Dimensions | Cluster Number |
| --------- | ------ | ---------- | -------------- |
| CoverType | 581012 | 54         | 7              |


## Baseline Performance

| Algorithm | Execution Time | Purity (correctness check) | Num of Clusters (correctness check) |
| --------- | -------------- | -------------------------- | ----------------------------------- |
| BIRCH     | 1081 ms        | 0.0817814                  | 57                                  |
| CluStream | 1001 ms        | 0.717314                   | 7                                   |
| EDMStream | 181 ms         | 0.893519                   | 14                                  |
| DStream   | 778 ms         | 0.770046                   | 101                                 |
| DenStream | 1045 ms        | 0.374782                   | 15                                  |
| SL-KMeans | 3082507 ms     | 0.171415                   | 7                                   |

Baseline performance is tested on CoverType dataset with 581012 points. 
- CPU: AMD Ryzen 7 5700U
- RAM: 24GB DDR4 3200MHz



## Reference

* **[SIGMOD 2023]** Xin Wang and Zhengru Wang and Zhenyu Wu and Shuhao Zhang and Xuanhua Shi and Li Lu. Data Stream Clustering: An In-depth Empirical Study, SIGMOD, 2023

```
@inproceedings{wang2023sesame,
	title        = {Data Stream Clustering: An In-depth Empirical Study},
	author       = {Xin Wang and Zhengru Wang and Zhenyu Wu and Shuhao Zhang and Xuanhua Shi and Li Lu},
	year         = 2023,
	booktitle    = {Proceedings of the 2023 International Conference on Management of Data (SIGMOD)},
	location     = {Seattle, WA, USA},
	publisher    = {Association for Computing Machinery},
	address      = {New York, NY, USA},
	series       = {SIGMOD '23},
	abbr         = {SIGMOD},
	bibtex_show  = {true},
	selected     = {true},
	pdf          = {papers/Sesame.pdf},
	code         = {https://github.com/intellistream/Sesame},
	doi	         = {10.1145/3589307},
    url          = {https://doi.org/10.1145/3589307}
}
```


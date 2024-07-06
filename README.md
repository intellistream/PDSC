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

## Real-world algorithms

| Algorithm  | Window Model | Outlier Detection | Summarizing Data Structure | Offline Refinement |
| ---------- | ------------ | ----------------- | -------------------------- | ------------------ |
| BIRCH      | LandmarkWM   | OutlierD          | CFT                        | ❌                  |
| CluStream  | LandmarkWM   | OutlierD-T        | MCs                        | ✅                  |
| DenStream  | DampedWM     | OutlierD-BT       | MCs                        | ✅                  |
| DStream    | DampedWM     | OutlierD-T        | Grids                      | ❌                  |
| StreamKM++ | LandmarkWM   | NoOutlierD        | CoreT                      | ✅                  |
| DBStream   | DampedWM     | OutlierD-T        | MCs                        | ✅                  |
| EDMStream  | DampedWM     | OutlierD-BT       | DPT                        | ❌                  |
| SL-KMeans  | SlidingWM    | NoOutlierD        | AMS                        | ❌                  |


## Datasets

| DataSet   | Length                                | Dimension | Cluster Number         |
| --------- | ------------------------------------- | --------- | ---------------------- |
| CoverType | 581012                                | 54        | 7                      |
| KDD-99    | 4898431                               | 41        | 23                     |
| Insects   | 905145                                | 33        | 24                     |
| Sensor    | 2219803                               | 5         | 55                     |
| EDS       | 45690, 100270, 150645, 200060, 245270 | 2         | 75, 145, 218, 289, 363 |
| ODS       | 94720,97360,100000                    | 2         | 90, 90, 90             |


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


# Opal
Open Platform Abstraction Layer

## Benchmarks

|               ns/op |                op/s |    err% |     total | benchmark
|--------------------:|--------------------:|--------:|----------:|:----------
|            1,515.67 |          659,775.15 |    0.9% |      0.19 | `Concatenate Paths`
|            3,752.38 |          266,497.53 |    0.5% |      0.45 | `Concatenate Paths With Up Reference`
|              156.47 |        6,390,821.00 |    3.1% |      0.02 | `Initialize Path With Root`
|              177.31 |        5,639,696.14 |    0.9% |      0.21 | `Initialize Path Relative`
|            2,307.37 |          433,393.47 |    1.6% |      0.29 | `Get User Profile Directory`
|            2,854.73 |          350,295.41 |    4.0% |      0.35 | `Get Current Directory`
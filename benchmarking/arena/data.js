window.BENCHMARK_DATA = {
  "lastUpdate": 1706645501073,
  "repoUrl": "https://github.com/ruse-ai/mitpokerbots-2024",
  "entries": {
    "Arena Benchmark": [
      {
        "commit": {
          "author": {
            "email": "philbeardsell@gmail.com",
            "name": "Philippe Beardsell",
            "username": "philqc"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "7436b5ad7a2ac70245f66de953616c677288083f",
          "message": "Check 100% flop OOP and fix psb (#39)\n\n* Check 100% flop OOP and fix psb\r\n\r\n* done\r\n\r\n* temp fix - increase build timeout\r\n\r\n* dont bet OOP turn with 2 cards",
          "timestamp": "2024-01-30T12:17:17-05:00",
          "tree_id": "541bb03a1034095f301094cb5678ee4a5f6b11ca",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/7436b5ad7a2ac70245f66de953616c677288083f"
        },
        "date": 1706635269973,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.0345,
            "unit": "bb/hand",
            "range": 0.7336
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 62.26,
            "unit": "bb/hand",
            "range": 3.0255
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 14.5005,
            "unit": "bb/hand",
            "range": 2.5654
          },
          {
            "name": "Results vs. Ourselves",
            "value": 1.851,
            "unit": "bb/hand",
            "range": 3.3156
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "philbeardsell@gmail.com",
            "name": "Philippe Beardsell",
            "username": "philqc"
          },
          "committer": {
            "email": "philbeardsell@gmail.com",
            "name": "Philippe Beardsell",
            "username": "philqc"
          },
          "distinct": true,
          "id": "d90284ea9a06b0c35d3d8e80ff41d8fd4cd8f33d",
          "message": "update config before submission",
          "timestamp": "2024-01-30T12:31:43-05:00",
          "tree_id": "29226eded8cc5270e4ac7ef851133300d5471e7b",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/d90284ea9a06b0c35d3d8e80ff41d8fd4cd8f33d"
        },
        "date": 1706636187452,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.118,
            "unit": "bb/hand",
            "range": 0.6688
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 55.558,
            "unit": "bb/hand",
            "range": 2.9331
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 14.502,
            "unit": "bb/hand",
            "range": 2.5861
          },
          {
            "name": "Results vs. Ourselves",
            "value": -6.8695,
            "unit": "bb/hand",
            "range": 3.6143
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "committer": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "id": "6b4c4b7671a6304340c35d64d60b97e2e0f3122a",
          "message": "Timer",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/40/commits/6b4c4b7671a6304340c35d64d60b97e2e0f3122a"
        },
        "date": 1706636297113,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.6875,
            "unit": "bb/hand",
            "range": 0.8083
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 62.524,
            "unit": "bb/hand",
            "range": 3.0267
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 15.919,
            "unit": "bb/hand",
            "range": 2.6524
          },
          {
            "name": "Results vs. Ourselves",
            "value": 2.4885,
            "unit": "bb/hand",
            "range": 3.8661
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "committer": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "id": "93df6cb02e977be2d043cf81234e1d435976cd54",
          "message": "Replace `std::sort` with `pdqsort`",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/41/commits/93df6cb02e977be2d043cf81234e1d435976cd54"
        },
        "date": 1706640500943,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 1.473,
            "unit": "bb/hand",
            "range": 0.8235
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 56.542,
            "unit": "bb/hand",
            "range": 2.985
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 17.8605,
            "unit": "bb/hand",
            "range": 2.7604
          },
          {
            "name": "Results vs. Ourselves",
            "value": 1.832,
            "unit": "bb/hand",
            "range": 3.5471
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "committer": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "id": "1017b9110963a67012eaab59cb7e464e8b7ac3ba",
          "message": "Timer",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/40/commits/1017b9110963a67012eaab59cb7e464e8b7ac3ba"
        },
        "date": 1706641917110,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.454,
            "unit": "bb/hand",
            "range": 1.0593
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 53.731,
            "unit": "bb/hand",
            "range": 2.9288
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 18.196,
            "unit": "bb/hand",
            "range": 2.8436
          },
          {
            "name": "Results vs. Ourselves",
            "value": -4.0205,
            "unit": "bb/hand",
            "range": 4.6157
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "committer": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "id": "fcf5313f7451e52972a61bad20198f130df00dd8",
          "message": "add preflop cache root node",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/42/commits/fcf5313f7451e52972a61bad20198f130df00dd8"
        },
        "date": 1706642191777,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.9555,
            "unit": "bb/hand",
            "range": 0.9599
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 59.7055,
            "unit": "bb/hand",
            "range": 3.0386
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 12.123,
            "unit": "bb/hand",
            "range": 3.0621
          },
          {
            "name": "Results vs. Ourselves",
            "value": 1.6155,
            "unit": "bb/hand",
            "range": 3.4912
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "93880894+alikhasi@users.noreply.github.com",
            "name": "alikhasi",
            "username": "alikhasi"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "e37968e5f186ca5f515851252651e10aa903f82e",
          "message": "Timer (#40)\n\n* add timer\r\n\r\n* remove time budget from the auction",
          "timestamp": "2024-01-30T14:20:48-05:00",
          "tree_id": "183d14f4c45baf31a151b28d9c44b64c70b604aa",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/e37968e5f186ca5f515851252651e10aa903f82e"
        },
        "date": 1706642684894,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 1.4095,
            "unit": "bb/hand",
            "range": 1.1573
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 59.921,
            "unit": "bb/hand",
            "range": 3.0061
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 18.7085,
            "unit": "bb/hand",
            "range": 2.8848
          },
          {
            "name": "Results vs. Ourselves",
            "value": -10.8255,
            "unit": "bb/hand",
            "range": 4.3714
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "committer": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "id": "4f261a2a07010e5468c952c442ee596912713ae1",
          "message": "add preflop cache root node",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/42/commits/4f261a2a07010e5468c952c442ee596912713ae1"
        },
        "date": 1706642859366,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -1.0355,
            "unit": "bb/hand",
            "range": 1.4248
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 57.621,
            "unit": "bb/hand",
            "range": 2.9867
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 17.146,
            "unit": "bb/hand",
            "range": 3.1505
          },
          {
            "name": "Results vs. Ourselves",
            "value": -5.1635,
            "unit": "bb/hand",
            "range": 3.861
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "committer": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "id": "4f261a2a07010e5468c952c442ee596912713ae1",
          "message": "add preflop cache root node",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/42/commits/4f261a2a07010e5468c952c442ee596912713ae1"
        },
        "date": 1706643293669,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.837,
            "unit": "bb/hand",
            "range": 1.5178
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 52.869,
            "unit": "bb/hand",
            "range": 2.9184
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 12.43,
            "unit": "bb/hand",
            "range": 3.2424
          },
          {
            "name": "Results vs. Ourselves",
            "value": 0.5455,
            "unit": "bb/hand",
            "range": 3.6168
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "committer": {
            "name": "ruse-ai",
            "username": "ruse-ai"
          },
          "id": "f1e88d07711c9c3f5fe8a4ed995d471a611eed5a",
          "message": "add preflop cache root node",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/42/commits/f1e88d07711c9c3f5fe8a4ed995d471a611eed5a"
        },
        "date": 1706645500167,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.492,
            "unit": "bb/hand",
            "range": 0.7079
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 53.676,
            "unit": "bb/hand",
            "range": 2.9232
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 17.9565,
            "unit": "bb/hand",
            "range": 2.6769
          },
          {
            "name": "Results vs. Ourselves",
            "value": -2.591,
            "unit": "bb/hand",
            "range": 3.9477
          }
        ]
      }
    ]
  }
}
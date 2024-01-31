window.BENCHMARK_DATA = {
  "lastUpdate": 1706720561405,
  "repoUrl": "https://github.com/ruse-ai/mitpokerbots-2024",
  "entries": {
    "Arena Benchmark": [
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
          "id": "73055d224fc7e050d4d3802dcb0c514a7757470f",
          "message": "Fix timing",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/44/commits/73055d224fc7e050d4d3802dcb0c514a7757470f"
        },
        "date": 1706658281100,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.7655,
            "unit": "bb/hand",
            "range": 0.7265
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 62.014,
            "unit": "bb/hand",
            "range": 3.0223
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 19.3365,
            "unit": "bb/hand",
            "range": 2.6936
          },
          {
            "name": "Results vs. Ourselves",
            "value": -4.3865,
            "unit": "bb/hand",
            "range": 3.5137
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
          "id": "ad177299b42c99eae3d8a492d07a727b1c804e01",
          "message": "Fix timing (#44)\n\n* not considering first 50 actions if they take more time\r\n\r\n* play based on some fixed ratio\r\n\r\n* remove print\r\n\r\n* change var name",
          "timestamp": "2024-01-30T18:45:27-05:00",
          "tree_id": "49686b59a5106568a39fbbe648c9f0014083d55f",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/ad177299b42c99eae3d8a492d07a727b1c804e01"
        },
        "date": 1706658645560,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.737,
            "unit": "bb/hand",
            "range": 0.6433
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 54.147,
            "unit": "bb/hand",
            "range": 2.9232
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 16.7815,
            "unit": "bb/hand",
            "range": 2.5804
          },
          {
            "name": "Results vs. Ourselves",
            "value": 3.297,
            "unit": "bb/hand",
            "range": 3.6273
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
          "id": "12b534a391d1791312e17bb0eec2c956a2e7e490",
          "message": "add preflop all in bot",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/46/commits/12b534a391d1791312e17bb0eec2c956a2e7e490"
        },
        "date": 1706669072525,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.889,
            "unit": "bb/hand",
            "range": 0.6355
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 58.451,
            "unit": "bb/hand",
            "range": 2.9825
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 18.175,
            "unit": "bb/hand",
            "range": 2.6023
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 5.281,
            "unit": "bb/hand",
            "range": 4.3671
          },
          {
            "name": "Results vs. Ourselves",
            "value": -5.388,
            "unit": "bb/hand",
            "range": 3.564
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
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "3d83398ed5750ad12b5f891524a6994814ca3182",
          "message": "add preflop all in bot (#46)",
          "timestamp": "2024-01-30T21:44:58-05:00",
          "tree_id": "afd9d18d2760ba72d199b218b8327031a963e68b",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/3d83398ed5750ad12b5f891524a6994814ca3182"
        },
        "date": 1706669441931,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.4075,
            "unit": "bb/hand",
            "range": 0.7029
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 60.92,
            "unit": "bb/hand",
            "range": 3.0063
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 16.9615,
            "unit": "bb/hand",
            "range": 2.4877
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 8.498,
            "unit": "bb/hand",
            "range": 4.4168
          },
          {
            "name": "Results vs. Ourselves",
            "value": -1.5895,
            "unit": "bb/hand",
            "range": 3.5993
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
          "id": "0dfa691c8093eb6fa1d80d529f431cf890d67c13",
          "message": "Optimize timer",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/48/commits/0dfa691c8093eb6fa1d80d529f431cf890d67c13"
        },
        "date": 1706718178489,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.102,
            "unit": "bb/hand",
            "range": 0.7254
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 60.659,
            "unit": "bb/hand",
            "range": 3.0033
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 14.1805,
            "unit": "bb/hand",
            "range": 2.66
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 1.526,
            "unit": "bb/hand",
            "range": 4.5452
          },
          {
            "name": "Results vs. Ourselves",
            "value": -0.0475,
            "unit": "bb/hand",
            "range": 3.3949
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
          "id": "bebccb87a589c9d59d10c979ab6752c26a73b71e",
          "message": "Optimize CFR (~20% increase in # of iterations)",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/47/commits/bebccb87a589c9d59d10c979ab6752c26a73b71e"
        },
        "date": 1706719005191,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.5345,
            "unit": "bb/hand",
            "range": 0.7031
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 56.8705,
            "unit": "bb/hand",
            "range": 2.9709
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 17.07,
            "unit": "bb/hand",
            "range": 2.6712
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 7.47,
            "unit": "bb/hand",
            "range": 4.3132
          },
          {
            "name": "Results vs. Ourselves",
            "value": 7.174,
            "unit": "bb/hand",
            "range": 3.5532
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
          "id": "0dfa691c8093eb6fa1d80d529f431cf890d67c13",
          "message": "Optimize timer",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/48/commits/0dfa691c8093eb6fa1d80d529f431cf890d67c13"
        },
        "date": 1706719381505,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.006,
            "unit": "bb/hand",
            "range": 0.6137
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 61.5235,
            "unit": "bb/hand",
            "range": 3.0334
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 20.7975,
            "unit": "bb/hand",
            "range": 2.7748
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 12.088,
            "unit": "bb/hand",
            "range": 4.3902
          },
          {
            "name": "Results vs. Ourselves",
            "value": -0.599,
            "unit": "bb/hand",
            "range": 3.4532
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
          "id": "01e644aff055b28784380ea0391a82dd795eedbe",
          "message": "Fixed update_exploits",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/49/commits/01e644aff055b28784380ea0391a82dd795eedbe"
        },
        "date": 1706720394962,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.965,
            "unit": "bb/hand",
            "range": 0.5845
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 9.5105,
            "unit": "bb/hand",
            "range": 3.4201
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 20.467,
            "unit": "bb/hand",
            "range": 2.6221
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 11.103,
            "unit": "bb/hand",
            "range": 4.4336
          },
          {
            "name": "Results vs. Ourselves",
            "value": -3.031,
            "unit": "bb/hand",
            "range": 3.613
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
          "id": "d8ff561154b63752a9124ca01207cbd70412e60b",
          "message": "Fixed update_exploits",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/49/commits/d8ff561154b63752a9124ca01207cbd70412e60b"
        },
        "date": 1706720445175,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 1.3355,
            "unit": "bb/hand",
            "range": 0.6389
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 6.494,
            "unit": "bb/hand",
            "range": 3.4782
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 17.7205,
            "unit": "bb/hand",
            "range": 2.7522
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 6.288,
            "unit": "bb/hand",
            "range": 4.3887
          },
          {
            "name": "Results vs. Ourselves",
            "value": 5.1595,
            "unit": "bb/hand",
            "range": 3.5683
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
          "id": "8aba80a21d5b7201968b0e271efb46e3187e3bc5",
          "message": "Fixed update_exploits",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/49/commits/8aba80a21d5b7201968b0e271efb46e3187e3bc5"
        },
        "date": 1706720560060,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.7105,
            "unit": "bb/hand",
            "range": 0.7177
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 6.9085,
            "unit": "bb/hand",
            "range": 3.3723
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 12.1485,
            "unit": "bb/hand",
            "range": 2.6598
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 7.712,
            "unit": "bb/hand",
            "range": 4.499
          },
          {
            "name": "Results vs. Ourselves",
            "value": -0.9755,
            "unit": "bb/hand",
            "range": 3.5711
          }
        ]
      }
    ]
  }
}
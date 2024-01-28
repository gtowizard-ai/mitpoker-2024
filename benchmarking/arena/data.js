window.BENCHMARK_DATA = {
  "lastUpdate": 1706471647083,
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
          "id": "34644ef981dae3d1010dac6bd6f392565eb5e132",
          "message": "Mccfr",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/27/commits/34644ef981dae3d1010dac6bd6f392565eb5e132"
        },
        "date": 1706295145760,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.2739,
            "unit": "bb/hand",
            "range": 0.4679
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 0.1858,
            "unit": "bb/hand",
            "range": 1.4749
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
          "id": "735d167e2c0a6808d0e03a49ec2f5c2adde55fa5",
          "message": "Main bot",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/29/commits/735d167e2c0a6808d0e03a49ec2f5c2adde55fa5"
        },
        "date": 1706299718229,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.34,
            "unit": "bb/hand",
            "range": 0.1216
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -40,
            "unit": "bb/hand",
            "range": 19.6946
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
          "id": "1269edba4c32409dd6e1d603b94c5167804243c5",
          "message": "Main bot",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/29/commits/1269edba4c32409dd6e1d603b94c5167804243c5"
        },
        "date": 1706299912398,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.28,
            "unit": "bb/hand",
            "range": 0.1422
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -62,
            "unit": "bb/hand",
            "range": 18.5744
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
          "id": "153802f8adf40e9968029bd0f86c50d6af23a4d6",
          "message": "Main bot",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/29/commits/153802f8adf40e9968029bd0f86c50d6af23a4d6"
        },
        "date": 1706300407789,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.2214,
            "unit": "bb/hand",
            "range": 0.0186
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -63.96,
            "unit": "bb/hand",
            "range": 2.6377
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
          "id": "2f8f6d851300d7b71caf7f11d988c3d301e29726",
          "message": "Main bot (#29)\n\n* initialize the classes\r\n\r\n* merge Ranges to code\r\n\r\n* only use regret to track strategy\r\n\r\n* bug fix\r\n\r\n* fix\r\n\r\n* add warm-up steps\r\n\r\n* fix\r\n\r\n* optimization\r\n\r\n* bug fix - remove reference members\r\n\r\n* update comment\r\n\r\n* add timing\r\n\r\n* bug fix - using of std::chrono\r\n\r\n* bug fix - using array instead of vector\r\n\r\n* terminal value - messy initialization\r\n\r\n* add terminal value computation\r\n\r\n* rename\r\n\r\n* rename\r\n\r\n* bug fix - blocker hands\r\n\r\n* fix\r\n\r\n* remove files\r\n\r\n* start the node evaluation section\r\n\r\n* using equity calculator\r\n\r\n* bug fix\r\n\r\n* bug fix\r\n\r\n* optimization\r\n\r\n* sampling fix\r\n\r\n* timer\r\n\r\n* timer\r\n\r\n* remove terminal check\r\n\r\n* add preflop eq tabels\r\n\r\n* wip\r\n\r\n* add test cases\r\n\r\n* merge\r\n\r\n* C++ version fix\r\n\r\n* add chrono header\r\n\r\n* add board to the state\r\n\r\n* convert to 3 cards\r\n\r\n* fix test case\r\n\r\n* wip\r\n\r\n* wip\r\n\r\n* avoid cfr\r\n\r\n* wip\r\n\r\n* use mccfr legal actions as suggested\r\n\r\n* wip\r\n\r\n---------\r\n\r\nCo-authored-by: alikhasi <alikhasi@ualberta.ca>",
          "timestamp": "2024-01-26T15:17:06-05:00",
          "tree_id": "1a12c2c3ae72b7ee65f6478328052ff323daabe3",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/2f8f6d851300d7b71caf7f11d988c3d301e29726"
        },
        "date": 1706300425020,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.186,
            "unit": "bb/hand",
            "range": 0.0188
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -67.72,
            "unit": "bb/hand",
            "range": 2.6219
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
          "id": "edd12e2cbb7dc52ad6494955777b6bfc1fbb4b96",
          "message": "Add preflop main bot",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/31/commits/edd12e2cbb7dc52ad6494955777b6bfc1fbb4b96"
        },
        "date": 1706309525589,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 13.323,
            "unit": "bb/hand",
            "range": 4.5874
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -21.9405,
            "unit": "bb/hand",
            "range": 5.2599
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
          "id": "c31b3fa4760f19d1f4c6a89cdbdea73fb2379bb3",
          "message": "Add preflop main bot",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/31/commits/c31b3fa4760f19d1f4c6a89cdbdea73fb2379bb3"
        },
        "date": 1706309666771,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 18.7085,
            "unit": "bb/hand",
            "range": 4.4396
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -19.3325,
            "unit": "bb/hand",
            "range": 5.3343
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
          "id": "9ae95595999c906dc7886da313f941fba8f071bf",
          "message": "Add preflop main bot (#31)\n\n* test\r\n\r\n* wip\r\n\r\n* dont solve turn\r\n\r\n* fix\r\n\r\n* preflop cache\r\n\r\n* try to respect time constraints\r\n\r\n* fix\r\n\r\n* wip\r\n\r\n* aethestic",
          "timestamp": "2024-01-26T17:53:20-05:00",
          "tree_id": "b1cdb04ff20bfe1afee28bc2eac505315014e9e8",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/9ae95595999c906dc7886da313f941fba8f071bf"
        },
        "date": 1706309769902,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 10.3155,
            "unit": "bb/hand",
            "range": 4.5557
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -6.1235,
            "unit": "bb/hand",
            "range": 5.4399
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
          "id": "b5a31f3153aad30a4ff52e2aeb3c4980977e7900",
          "message": "CFR two limited steps",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/32/commits/b5a31f3153aad30a4ff52e2aeb3c4980977e7900"
        },
        "date": 1706468719998,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.6,
            "unit": "bb/hand",
            "range": 0.0823
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -51.518,
            "unit": "bb/hand",
            "range": 5.182
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
          "id": "a77f2945e3efe7672ae21b70f0998e13b047c4ff",
          "message": "CFR two limited steps",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/32/commits/a77f2945e3efe7672ae21b70f0998e13b047c4ff"
        },
        "date": 1706471646296,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.0955,
            "unit": "bb/hand",
            "range": 0.0539
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -41.732,
            "unit": "bb/hand",
            "range": 5.2121
          }
        ]
      }
    ]
  }
}
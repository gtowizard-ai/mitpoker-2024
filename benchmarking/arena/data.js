window.BENCHMARK_DATA = {
  "lastUpdate": 1706473387174,
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
          "id": "fa3daa51b2e4bc04d5f9a416a79a52a029c4652a",
          "message": "CFR two limited steps",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/32/commits/fa3daa51b2e4bc04d5f9a416a79a52a029c4652a"
        },
        "date": 1706472237026,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.248,
            "unit": "bb/hand",
            "range": 0.0499
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -35.1335,
            "unit": "bb/hand",
            "range": 5.2415
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
          "id": "3b23816a57d17019ca41309494c46a9a8201e2c8",
          "message": "CFR two limited steps",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/32/commits/3b23816a57d17019ca41309494c46a9a8201e2c8"
        },
        "date": 1706472341836,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.1705,
            "unit": "bb/hand",
            "range": 0.0487
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -40.5165,
            "unit": "bb/hand",
            "range": 5.3204
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
          "id": "1b1aa4cb35d93015aaf801f68abaae0b8e1edd3d",
          "message": "CFR two limited steps",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/32/commits/1b1aa4cb35d93015aaf801f68abaae0b8e1edd3d"
        },
        "date": 1706472579023,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.131,
            "unit": "bb/hand",
            "range": 0.0513
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -39.922,
            "unit": "bb/hand",
            "range": 5.2834
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
          "id": "4163eeba79901ef3c7440b876c93e409c19b9e32",
          "message": "CFR two limited steps",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/32/commits/4163eeba79901ef3c7440b876c93e409c19b9e32"
        },
        "date": 1706473386248,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.1135,
            "unit": "bb/hand",
            "range": 0.0516
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -48.723,
            "unit": "bb/hand",
            "range": 5.1936
          }
        ]
      }
    ]
  }
}
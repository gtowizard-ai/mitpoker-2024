window.BENCHMARK_DATA = {
  "lastUpdate": 1706281755304,
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
          "id": "7670746f0a4f163bbdb5b1e3a8e3aacce7d2e3df",
          "message": "Fix duplicate definition of `PokerHand`",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/18/commits/7670746f0a4f163bbdb5b1e3a8e3aacce7d2e3df"
        },
        "date": 1706215099124,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.0186,
            "unit": "bb/hand",
            "range": 0.4686
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -2.3681,
            "unit": "bb/hand",
            "range": 1.4753
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "46653792+GrahamZugPitt@users.noreply.github.com",
            "name": "GrahamZugPitt",
            "username": "GrahamZugPitt"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "000b87e196f968e19d6c6bf7763face90c24b810",
          "message": "Recieve bid (#17)\n\n* recieve_bid started\r\n\r\n* Spelling is hard\r\n\r\n* Phil's comments\r\n\r\n* Phil comment number 2",
          "timestamp": "2024-01-25T15:40:31-05:00",
          "tree_id": "bcdcb61aa1daa5f765951c92ae9266045403e2f5",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/000b87e196f968e19d6c6bf7763face90c24b810"
        },
        "date": 1706215315343,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.1684,
            "unit": "bb/hand",
            "range": 0.4776
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 0.3509,
            "unit": "bb/hand",
            "range": 1.4753
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
          "id": "8d7753ea044460e5bf46a83a2424b805e3766188",
          "message": "Add CFV/equity calculator",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/19/commits/8d7753ea044460e5bf46a83a2424b805e3766188"
        },
        "date": 1706232963925,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.6803,
            "unit": "bb/hand",
            "range": 0.4743
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -0.2555,
            "unit": "bb/hand",
            "range": 1.4662
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
          "id": "6799db2c5d2abf4ffeca4c86c9df408c5854189e",
          "message": "Add CFV/equity calculator",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/19/commits/6799db2c5d2abf4ffeca4c86c9df408c5854189e"
        },
        "date": 1706236120981,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.3637,
            "unit": "bb/hand",
            "range": 0.4585
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 0.7436,
            "unit": "bb/hand",
            "range": 1.4765
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "oinari17@gmail.com",
            "name": "Wataru Inariba",
            "username": "b-inary"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "55f53e6f2a36dbc1c04de0dff8b6b81ae2aa5201",
          "message": "Add CFV/equity calculator (#19)\n\n* WIP: Added `compute_equities` function\r\n\r\n* Eliminated warnings\r\n\r\n* tests for equity calculator\r\n\r\n* Added benchmark script\r\n\r\n2137.47 / 1993.73 / 1047.44 [calls/s]\r\n\r\n* wip\r\n\r\n* typo\r\n\r\n* fix\r\n\r\n* fix error\r\n\r\n* Made `compute_cfvs_river` public\r\n\r\n* fix\r\n\r\n* fix range test\r\n\r\n* cleanup a bit\r\n\r\n* temp fix - increase build time\r\n\r\n* temp fix - increase build time\r\n\r\n---------\r\n\r\nCo-authored-by: Philippe Beardsell <philbeardsell@gmail.com>",
          "timestamp": "2024-01-25T21:33:13-05:00",
          "tree_id": "fef3ffe92738f2b6a68e8ee488e723a8f7b9571f",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/55f53e6f2a36dbc1c04de0dff8b6b81ae2aa5201"
        },
        "date": 1706236487025,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.87,
            "unit": "bb/hand",
            "range": 0.4658
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -2.7226,
            "unit": "bb/hand",
            "range": 1.4685
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
          "id": "4a0788b85199e95db25d76ba49150a38660e491d",
          "message": "Receive bid",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/24/commits/4a0788b85199e95db25d76ba49150a38660e491d"
        },
        "date": 1706279353313,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.31,
            "unit": "bb/hand",
            "range": 0.4633
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 1.0311,
            "unit": "bb/hand",
            "range": 1.4733
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
          "id": "43b5b1dd1a5a84f286e600ab4a08d5feca6991b7",
          "message": "Equity optimization no highway",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/25/commits/43b5b1dd1a5a84f286e600ab4a08d5feca6991b7"
        },
        "date": 1706280117646,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.0853,
            "unit": "bb/hand",
            "range": 0.4646
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -0.8928,
            "unit": "bb/hand",
            "range": 1.468
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
          "id": "a8dad227397c02c5c5953cd2c31d7d038d170e52",
          "message": "Equity optimization no highway (#25)\n\n* Use `VQSort` instead of `std::sort`\r\n\r\n* Remove validations in `PokerHand` (30%~ faster)\r\n\r\n* Fix `PokerHand` test\r\n\r\n* Optimize river equity\r\n\r\n* Tentative fix for Clang\r\n\r\n* Fix\r\n\r\n* no highway\r\n\r\n---------\r\n\r\nCo-authored-by: Wataru Inariba <oinari17@gmail.com>",
          "timestamp": "2024-01-26T09:42:15-05:00",
          "tree_id": "d2cc0eb8fd6de9dc3efb60f137cce0fdacc1e5e3",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/a8dad227397c02c5c5953cd2c31d7d038d170e52"
        },
        "date": 1706280318638,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.2488,
            "unit": "bb/hand",
            "range": 0.4735
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -1.6768,
            "unit": "bb/hand",
            "range": 1.4781
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
          "id": "cc1076cc7d437ce9dcbaab621ef747f9e5b3ea78",
          "message": "Receive bid",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/24/commits/cc1076cc7d437ce9dcbaab621ef747f9e5b3ea78"
        },
        "date": 1706281438372,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.1249,
            "unit": "bb/hand",
            "range": 0.4732
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -0.5733,
            "unit": "bb/hand",
            "range": 1.4786
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
          "id": "b01e6cbf3d2c9e599a82efa4b462bf753c066743",
          "message": "Receive bid",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/24/commits/b01e6cbf3d2c9e599a82efa4b462bf753c066743"
        },
        "date": 1706281753849,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.3724,
            "unit": "bb/hand",
            "range": 0.4636
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -1.4097,
            "unit": "bb/hand",
            "range": 1.4689
          }
        ]
      }
    ]
  }
}
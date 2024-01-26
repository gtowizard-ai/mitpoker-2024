window.BENCHMARK_DATA = {
  "lastUpdate": 1706293160454,
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
          "id": "3dd36037f5bc0b1180980b16ab1580c622169bad",
          "message": "Receive bid (#24)\n\n* Just a little checkpoint.\r\n\r\n* Finished UpdateExploits Test\r\n\r\n* Removed excessive import and inaccurate comment\r\n\r\n* Phils comments",
          "timestamp": "2024-01-26T10:11:26-05:00",
          "tree_id": "0768e51542bb48eda78d9e10f076611f11d87eaf",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/3dd36037f5bc0b1180980b16ab1580c622169bad"
        },
        "date": 1706282053460,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.3508,
            "unit": "bb/hand",
            "range": 0.4793
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 0.0853,
            "unit": "bb/hand",
            "range": 1.471
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
          "id": "814d9c88ce6ca3c96ca1e78f23c37ccbaf64307c",
          "message": "add preflop eq tabels",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/26/commits/814d9c88ce6ca3c96ca1e78f23c37ccbaf64307c"
        },
        "date": 1706287867072,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.0486,
            "unit": "bb/hand",
            "range": 0.4618
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -0.3115,
            "unit": "bb/hand",
            "range": 1.476
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
          "id": "70046c3519aace3e189f5c40961df31e18eaf821",
          "message": "Add Preflop EQ Table",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/26/commits/70046c3519aace3e189f5c40961df31e18eaf821"
        },
        "date": 1706290789935,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.6371,
            "unit": "bb/hand",
            "range": 0.476
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -0.5836,
            "unit": "bb/hand",
            "range": 1.4828
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
          "id": "dfdc6484d053e6a45a4fe53f1cf0d1469af09402",
          "message": "Add Preflop EQ Table",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/26/commits/dfdc6484d053e6a45a4fe53f1cf0d1469af09402"
        },
        "date": 1706292302774,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.2742,
            "unit": "bb/hand",
            "range": 0.4711
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -0.6509,
            "unit": "bb/hand",
            "range": 1.4764
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
          "id": "c70164078420cf33154902ef7fb3ecc7425c1c86",
          "message": "Mccfr",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/27/commits/c70164078420cf33154902ef7fb3ecc7425c1c86"
        },
        "date": 1706292457667,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0,
            "unit": "bb/hand",
            "range": 0.005
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 0,
            "unit": "bb/hand",
            "range": 0.005
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
          "id": "b4b682b3a682db23062c6a430de973fa967ae455",
          "message": "Mccfr",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/27/commits/b4b682b3a682db23062c6a430de973fa967ae455"
        },
        "date": 1706292668339,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0,
            "unit": "bb/hand",
            "range": 0.005
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 0,
            "unit": "bb/hand",
            "range": 0.005
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
          "id": "badc9b71382a9b5deece841c0942240a8b07ffe2",
          "message": "Mccfr",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/27/commits/badc9b71382a9b5deece841c0942240a8b07ffe2"
        },
        "date": 1706293159607,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0,
            "unit": "bb/hand",
            "range": 0.005
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 0,
            "unit": "bb/hand",
            "range": 0.005
          }
        ]
      }
    ]
  }
}
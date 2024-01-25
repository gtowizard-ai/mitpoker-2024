window.BENCHMARK_DATA = {
  "lastUpdate": 1706203835680,
  "repoUrl": "https://github.com/ruse-ai/mitpokerbots-2024",
  "entries": {
    "Arena Benchmark": [
      {
        "commit": {
          "author": {
            "email": "oinari17@gmail.com",
            "name": "Wataru Inariba",
            "username": "b-inary"
          },
          "committer": {
            "email": "oinari17@gmail.com",
            "name": "Wataru Inariba",
            "username": "b-inary"
          },
          "distinct": true,
          "id": "53d526eea825424e97ba11a5a2114cdad913f9f1",
          "message": "Revert \"Revert \"Fix hand collision detection / Minor optimizations (#11)\"\"\n\nThis reverts commit dd48d5ae922494cad4f67920902fda043e564e07.",
          "timestamp": "2024-01-26T01:16:25+09:00",
          "tree_id": "37673c12cfddb62b26435b1ed4d962421678e895",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/53d526eea825424e97ba11a5a2114cdad913f9f1"
        },
        "date": 1706199648682,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.4259,
            "unit": "bb/hand",
            "range": 0.0066
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -2.0135,
            "unit": "bb/hand",
            "range": 1.4681
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
          "id": "72105867c9c91ebf8eba7352f247631beef66ac3",
          "message": "Add hand evaluator that supports 8 cards",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/12/commits/72105867c9c91ebf8eba7352f247631beef66ac3"
        },
        "date": 1706200350070,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.6677,
            "unit": "bb/hand",
            "range": 0.4781
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 0.19,
            "unit": "bb/hand",
            "range": 1.4769
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
          "id": "9491cbb0229826b6fcd900a2806b68fcb59092e5",
          "message": "initial auction commit",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/13/commits/9491cbb0229826b6fcd900a2806b68fcb59092e5"
        },
        "date": 1706200670612,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.421,
            "unit": "bb/hand",
            "range": 0.0066
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -1.4834,
            "unit": "bb/hand",
            "range": 1.4652
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
          "id": "954f858c5c64be2387286fc3060d0495ff26112b",
          "message": "Merge pull request #13 from ruse-ai/auction_initial\n\ninitial auction commit",
          "timestamp": "2024-01-25T11:45:14-05:00",
          "tree_id": "7e056380eab8a3099f9520289a593bcab4f00851",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/954f858c5c64be2387286fc3060d0495ff26112b"
        },
        "date": 1706201367780,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.4132,
            "unit": "bb/hand",
            "range": 0.0066
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 0.3379,
            "unit": "bb/hand",
            "range": 1.4698
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
          "id": "f18314ebc02f237b105e42a7c297605210665431",
          "message": "addressing Phil's comments",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/14/commits/f18314ebc02f237b105e42a7c297605210665431"
        },
        "date": 1706202957129,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.4181,
            "unit": "bb/hand",
            "range": 0.0066
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -1.6537,
            "unit": "bb/hand",
            "range": 1.4647
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
          "id": "290ebefcf1d253af0ac92527d108a4eb5304ca00",
          "message": "addressing Phil's comments",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/14/commits/290ebefcf1d253af0ac92527d108a4eb5304ca00"
        },
        "date": 1706203251490,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.4076,
            "unit": "bb/hand",
            "range": 0.0067
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -0.5166,
            "unit": "bb/hand",
            "range": 1.4778
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
          "id": "95c0992abce16dcdbade1b7b0bf564ee23950e8b",
          "message": "Add hand evaluator that supports 8 cards",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/12/commits/95c0992abce16dcdbade1b7b0bf564ee23950e8b"
        },
        "date": 1706203342854,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.4159,
            "unit": "bb/hand",
            "range": 0.0066
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -1.7541,
            "unit": "bb/hand",
            "range": 1.4767
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
          "id": "290ebefcf1d253af0ac92527d108a4eb5304ca00",
          "message": "Phils comments",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/15/commits/290ebefcf1d253af0ac92527d108a4eb5304ca00"
        },
        "date": 1706203440770,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.4178,
            "unit": "bb/hand",
            "range": 0.0066
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -0.6159,
            "unit": "bb/hand",
            "range": 1.4734
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
          "id": "da28fc43fbf74df06a8c5a48df4313418b9622cd",
          "message": "Add hand evaluator that supports 8 cards",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/12/commits/da28fc43fbf74df06a8c5a48df4313418b9622cd"
        },
        "date": 1706203548106,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.4224,
            "unit": "bb/hand",
            "range": 0.0066
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 0.0977,
            "unit": "bb/hand",
            "range": 1.4749
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
          "id": "67bc7512a3e56dc41d6236b97d1fe9e0e00ccec1",
          "message": "Phils comments (#15)\n\n* addressing Phil's comments\r\n\r\n* removed excessive import",
          "timestamp": "2024-01-25T12:29:11-05:00",
          "tree_id": "db53dbea6e7ebb8b0bb44c1886af031e0f89946f",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/67bc7512a3e56dc41d6236b97d1fe9e0e00ccec1"
        },
        "date": 1706203834711,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.6061,
            "unit": "bb/hand",
            "range": 0.4784
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -0.3038,
            "unit": "bb/hand",
            "range": 1.4682
          }
        ]
      }
    ]
  }
}
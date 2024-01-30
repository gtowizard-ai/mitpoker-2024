window.BENCHMARK_DATA = {
  "lastUpdate": 1706631625386,
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
          "id": "ac2d97aadbd02557d0b0cf518e210e8311fac66d",
          "message": "Average equity of 3rd card bid strategy",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/34/commits/ac2d97aadbd02557d0b0cf518e210e8311fac66d"
        },
        "date": 1706570026782,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.649,
            "unit": "bb/hand",
            "range": 0.1303
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -59.4395,
            "unit": "bb/hand",
            "range": 5.9
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 6.937,
            "unit": "bb/hand",
            "range": 3.1213
          },
          {
            "name": "Results vs. Ourselves",
            "value": -4.4525,
            "unit": "bb/hand",
            "range": 4.3933
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
          "id": "02ad4bb27e0298931207818c097b8899343f1471",
          "message": "Average equity of 3rd card bid strategy (#34)\n\n* initial get_bid commit\r\n\r\n* Added equity based bidding\r\n\r\n* snake to camel\r\n\r\n* Fixed arithmetic mistake\r\n\r\n* checkpoint\r\n\r\n* Removed old slow bidding\r\n\r\n* snakkkkeeee\r\n\r\n* wip\r\n\r\n* change name file\r\n\r\n* add equity bid base on average equity 3rd card\r\n\r\n* add test integrity of precomputed table\r\n\r\n---------\r\n\r\nCo-authored-by: GrahamZugPitt <GVZ3@pitt.edu>",
          "timestamp": "2024-01-29T18:10:36-05:00",
          "tree_id": "8eecc361da6c6174d25eca7164c58ec551bf26c2",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/02ad4bb27e0298931207818c097b8899343f1471"
        },
        "date": 1706570157134,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.7165,
            "unit": "bb/hand",
            "range": 0.1302
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -69.6275,
            "unit": "bb/hand",
            "range": 5.7972
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 14.5685,
            "unit": "bb/hand",
            "range": 3.3748
          },
          {
            "name": "Results vs. Ourselves",
            "value": -0.682,
            "unit": "bb/hand",
            "range": 4.367
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
          "id": "ff26ce253777d1b832da9a2125fe0b5556c77b77",
          "message": "Equity optimization",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/35/commits/ff26ce253777d1b832da9a2125fe0b5556c77b77"
        },
        "date": 1706570364173,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.581,
            "unit": "bb/hand",
            "range": 0.1164
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -65.8385,
            "unit": "bb/hand",
            "range": 5.8519
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 12.7315,
            "unit": "bb/hand",
            "range": 3.2919
          },
          {
            "name": "Results vs. Ourselves",
            "value": -3.0465,
            "unit": "bb/hand",
            "range": 3.3486
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
          "id": "39c7665e831076344dc8e03ffe450d14f80586cc",
          "message": "Equity optimization (#35)\n\n* Add fold CFV calculators (2v3, 3v2, 3v3)\r\n\r\n* WIP\r\n\r\n* Tests passed\r\n\r\n* Fix",
          "timestamp": "2024-01-30T08:19:42+09:00",
          "tree_id": "8af987a826e8abbd50e53c2a685367df686b7567",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/39c7665e831076344dc8e03ffe450d14f80586cc"
        },
        "date": 1706570604866,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.552,
            "unit": "bb/hand",
            "range": 0.1142
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -62.4495,
            "unit": "bb/hand",
            "range": 5.8995
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 16.89,
            "unit": "bb/hand",
            "range": 3.3218
          },
          {
            "name": "Results vs. Ourselves",
            "value": -0.188,
            "unit": "bb/hand",
            "range": 3.4856
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
          "id": "0230abc79b18211bc6c5eeaa91a4110a6170c10e",
          "message": "Exploits",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/36/commits/0230abc79b18211bc6c5eeaa91a4110a6170c10e"
        },
        "date": 1706627989565,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.9275,
            "unit": "bb/hand",
            "range": 0.1384
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 59.256,
            "unit": "bb/hand",
            "range": 3.0611
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 10.3365,
            "unit": "bb/hand",
            "range": 3.2533
          },
          {
            "name": "Results vs. Ourselves",
            "value": -1.898,
            "unit": "bb/hand",
            "range": 3.2935
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
          "id": "c20d7456bdef34f7c83ce3f1017b9022f38a1768",
          "message": "Exploits",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/36/commits/c20d7456bdef34f7c83ce3f1017b9022f38a1768"
        },
        "date": 1706628449833,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.6245,
            "unit": "bb/hand",
            "range": 0.1334
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 60.7225,
            "unit": "bb/hand",
            "range": 3.0617
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 8.726,
            "unit": "bb/hand",
            "range": 3.2378
          },
          {
            "name": "Results vs. Ourselves",
            "value": 6.435,
            "unit": "bb/hand",
            "range": 3.5175
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
          "id": "f53628fa6b0e0c20166aeddb879b68ce2a00ec40",
          "message": "Exploits",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/36/commits/f53628fa6b0e0c20166aeddb879b68ce2a00ec40"
        },
        "date": 1706630037696,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.7905,
            "unit": "bb/hand",
            "range": 0.1336
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 58.6455,
            "unit": "bb/hand",
            "range": 3.0121
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 17.6905,
            "unit": "bb/hand",
            "range": 3.2312
          },
          {
            "name": "Results vs. Ourselves",
            "value": -5.713,
            "unit": "bb/hand",
            "range": 3.3329
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
          "id": "7f0f8fad7f8cf6c2898379025a86eb77bdf8e794",
          "message": "Sloppy CFVs calculation on flop/turn",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/37/commits/7f0f8fad7f8cf6c2898379025a86eb77bdf8e794"
        },
        "date": 1706630148138,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.037,
            "unit": "bb/hand",
            "range": 0.4795
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 11.589,
            "unit": "bb/hand",
            "range": 3.4663
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 17.9515,
            "unit": "bb/hand",
            "range": 2.4651
          },
          {
            "name": "Results vs. Ourselves",
            "value": -3.644,
            "unit": "bb/hand",
            "range": 3.615
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
          "id": "22d05d4e8d30663c0fe36c98e6a498245a0c0e74",
          "message": "Exploits",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/36/commits/22d05d4e8d30663c0fe36c98e6a498245a0c0e74"
        },
        "date": 1706631497381,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.9825,
            "unit": "bb/hand",
            "range": 0.1308
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 60.4895,
            "unit": "bb/hand",
            "range": 3.0414
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 17.3005,
            "unit": "bb/hand",
            "range": 3.1818
          },
          {
            "name": "Results vs. Ourselves",
            "value": -3.04,
            "unit": "bb/hand",
            "range": 3.2648
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
          "id": "b1fd260c30a360f7519e0b1689b4e8a40f0fbdbb",
          "message": "Sloppy CFVs calculation on flop/turn (#37)",
          "timestamp": "2024-01-30T11:14:17-05:00",
          "tree_id": "9ca6bd2d71beeec65ded13738ac2621d343111f0",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/b1fd260c30a360f7519e0b1689b4e8a40f0fbdbb"
        },
        "date": 1706631623249,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.26,
            "unit": "bb/hand",
            "range": 0.5154
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 78.448,
            "unit": "bb/hand",
            "range": 3.1373
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 17.4785,
            "unit": "bb/hand",
            "range": 2.4791
          },
          {
            "name": "Results vs. Ourselves",
            "value": -1.092,
            "unit": "bb/hand",
            "range": 3.7127
          }
        ]
      }
    ]
  }
}
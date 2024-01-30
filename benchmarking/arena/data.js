window.BENCHMARK_DATA = {
  "lastUpdate": 1706636300241,
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
          "id": "fd791bc44fe9f2c97be93a2c631e673bfeaed0de",
          "message": "Hand eqs",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/38/commits/fd791bc44fe9f2c97be93a2c631e673bfeaed0de"
        },
        "date": 1706632382411,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.1355,
            "unit": "bb/hand",
            "range": 0.5446
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 71.803,
            "unit": "bb/hand",
            "range": 3.0946
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 18.6455,
            "unit": "bb/hand",
            "range": 2.6011
          },
          {
            "name": "Results vs. Ourselves",
            "value": -3.4675,
            "unit": "bb/hand",
            "range": 3.728
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
          "id": "aa33062ab6b7e89b0400aafd3526ea4c8e448db9",
          "message": "Hand eqs (#38)\n\n* wip\r\n\r\n* wip\r\n\r\n* wip\r\n\r\n* finished\r\n\r\n* increase cushion time manager\r\n\r\n* add data\r\n\r\n* add equity third card",
          "timestamp": "2024-01-30T11:35:14-05:00",
          "tree_id": "3710248217742379115d32852e1543b471503c12",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/aa33062ab6b7e89b0400aafd3526ea4c8e448db9"
        },
        "date": 1706632745315,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.404,
            "unit": "bb/hand",
            "range": 0.5372
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 75.829,
            "unit": "bb/hand",
            "range": 3.118
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 15.939,
            "unit": "bb/hand",
            "range": 2.5849
          },
          {
            "name": "Results vs. Ourselves",
            "value": -2.0575,
            "unit": "bb/hand",
            "range": 3.5338
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
          "id": "d6d5229e7fcccd522b21cf2fefab83a536a20433",
          "message": "Check 100% flop OOP and fix psb",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/39/commits/d6d5229e7fcccd522b21cf2fefab83a536a20433"
        },
        "date": 1706634257703,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.3915,
            "unit": "bb/hand",
            "range": 0.6525
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 57.573,
            "unit": "bb/hand",
            "range": 2.9771
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 19.057,
            "unit": "bb/hand",
            "range": 2.5711
          },
          {
            "name": "Results vs. Ourselves",
            "value": 4.636,
            "unit": "bb/hand",
            "range": 3.5568
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
          "id": "3b45ad6e310ad61ce61b83fb5f8663b0a37ce759",
          "message": "Check 100% flop OOP and fix psb",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/39/commits/3b45ad6e310ad61ce61b83fb5f8663b0a37ce759"
        },
        "date": 1706634501311,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 3.576,
            "unit": "bb/hand",
            "range": 0.7982
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 50.407,
            "unit": "bb/hand",
            "range": 2.8833
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 18.1585,
            "unit": "bb/hand",
            "range": 2.6261
          },
          {
            "name": "Results vs. Ourselves",
            "value": 1.48,
            "unit": "bb/hand",
            "range": 3.5328
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
      }
    ]
  }
}
window.BENCHMARK_DATA = {
  "lastUpdate": 1706757817197,
  "repoUrl": "https://github.com/ruse-ai/mitpokerbots-2024",
  "entries": {
    "Arena Benchmark": [
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
          "id": "8b266ad0697a162b53d439b46b329d10f287fb8d",
          "message": "Fixed update_exploits (#49)\n\n* Fixed update_exploits\r\n\r\n* removed iostream\r\n\r\n* fixed pre commit\r\n\r\n* Fixed auction for real this time.\r\n\r\n* precommit",
          "timestamp": "2024-01-31T16:59:59-05:00",
          "tree_id": "4e12f046a0f4cddd1f5e65333645d05381ae30fc",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/8b266ad0697a162b53d439b46b329d10f287fb8d"
        },
        "date": 1706738822260,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.142,
            "unit": "bb/hand",
            "range": 0.5713
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 58.087,
            "unit": "bb/hand",
            "range": 2.9678
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 20.1005,
            "unit": "bb/hand",
            "range": 2.628
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 12.276,
            "unit": "bb/hand",
            "range": 4.3114
          },
          {
            "name": "Results vs. Ourselves",
            "value": -1.011,
            "unit": "bb/hand",
            "range": 3.491
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
          "id": "b67668999b076c289cce985ab63b4e071fc64333",
          "message": "Optimize CFR (~20% increase in # of iterations) (#47)\n\n* Optimize CFR (~20% increase in # of iterations)\r\n\r\n* Fix\r\n\r\n* Fix",
          "timestamp": "2024-02-01T07:02:46+09:00",
          "tree_id": "08993b40aee078c9154fb1fb5fcaa2fdff2911fa",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/b67668999b076c289cce985ab63b4e071fc64333"
        },
        "date": 1706738881770,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 1.4735,
            "unit": "bb/hand",
            "range": 0.598
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 58.016,
            "unit": "bb/hand",
            "range": 2.9776
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 14.496,
            "unit": "bb/hand",
            "range": 2.5334
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 10.674,
            "unit": "bb/hand",
            "range": 4.2971
          },
          {
            "name": "Results vs. Ourselves",
            "value": 5.013,
            "unit": "bb/hand",
            "range": 3.4428
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
          "id": "3aa24241a9073d329f08d3c30d41306fa2759660",
          "message": "Timer improvement wataru (#50)\n\n* optimization\r\n\r\n* More improvements\r\n\r\n---------\r\n\r\nCo-authored-by: alikhasi <alikhasi@ualberta.ca>\r\nCo-authored-by: Wataru Inariba <oinari17@gmail.com>",
          "timestamp": "2024-02-01T07:08:53+09:00",
          "tree_id": "ae35a1e9056f728e34c0456c690db99204b22dd0",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/3aa24241a9073d329f08d3c30d41306fa2759660"
        },
        "date": 1706739257022,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 1.768,
            "unit": "bb/hand",
            "range": 0.854
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 60.51,
            "unit": "bb/hand",
            "range": 3.0014
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 19.23,
            "unit": "bb/hand",
            "range": 2.6192
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 7.504,
            "unit": "bb/hand",
            "range": 4.4412
          },
          {
            "name": "Results vs. Ourselves",
            "value": 1.9775,
            "unit": "bb/hand",
            "range": 3.5482
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
          "id": "b42283420ce25a9b549ff5a83a4bcca424b740eb",
          "message": "Preflop eqs 3 cards",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/51/commits/b42283420ce25a9b549ff5a83a4bcca424b740eb"
        },
        "date": 1706748351303,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 1.927,
            "unit": "bb/hand",
            "range": 0.6596
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 59.318,
            "unit": "bb/hand",
            "range": 3.0182
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 16.338,
            "unit": "bb/hand",
            "range": 2.7795
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 14.856,
            "unit": "bb/hand",
            "range": 4.409
          },
          {
            "name": "Results vs. Ourselves",
            "value": 0.5895,
            "unit": "bb/hand",
            "range": 3.4876
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
          "id": "1c7060cf1cae0235e388e632379ecb6a87ba103f",
          "message": "Preflop eqs 3 cards",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/51/commits/1c7060cf1cae0235e388e632379ecb6a87ba103f"
        },
        "date": 1706748473913,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.2615,
            "unit": "bb/hand",
            "range": 0.6311
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 54.077,
            "unit": "bb/hand",
            "range": 2.9202
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 15.3405,
            "unit": "bb/hand",
            "range": 2.8413
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 5.632,
            "unit": "bb/hand",
            "range": 4.3667
          },
          {
            "name": "Results vs. Ourselves",
            "value": -1.8125,
            "unit": "bb/hand",
            "range": 3.3801
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
          "id": "d3ddfcd4cd5c6dd7736e47c369fe9c9a675eac7e",
          "message": "Preflop eqs 3 cards (#51)\n\n* delete eqs - not needed\r\n\r\n* wip\r\n\r\n* better approximation\r\n\r\n* done\r\n\r\n* one more test",
          "timestamp": "2024-01-31T19:49:22-05:00",
          "tree_id": "af4448fa052626ead0a0cdf95bf7d897df9a3df0",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/d3ddfcd4cd5c6dd7736e47c369fe9c9a675eac7e"
        },
        "date": 1706748826038,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 3.129,
            "unit": "bb/hand",
            "range": 0.8522
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 55.352,
            "unit": "bb/hand",
            "range": 2.9463
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 15.4795,
            "unit": "bb/hand",
            "range": 2.7606
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 8.447,
            "unit": "bb/hand",
            "range": 4.3345
          },
          {
            "name": "Results vs. Ourselves",
            "value": 1.419,
            "unit": "bb/hand",
            "range": 3.5712
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
          "id": "ac01674e310d7debf78333fca571350817b7321d",
          "message": "Flop/Turn CFVs heuristic",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/52/commits/ac01674e310d7debf78333fca571350817b7321d"
        },
        "date": 1706753619370,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.6925,
            "unit": "bb/hand",
            "range": 1.4528
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 58.6585,
            "unit": "bb/hand",
            "range": 3.0488
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 13.8995,
            "unit": "bb/hand",
            "range": 3.5797
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 4.235,
            "unit": "bb/hand",
            "range": 4.3176
          },
          {
            "name": "Results vs. Ourselves",
            "value": 8.292,
            "unit": "bb/hand",
            "range": 6.1273
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
          "id": "2142e715fd1af6d9266b3e53daf566391bdbe4f9",
          "message": "wip",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/53/commits/2142e715fd1af6d9266b3e53daf566391bdbe4f9"
        },
        "date": 1706756884244,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.75,
            "unit": "bb/hand",
            "range": 1.1002
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 52.795,
            "unit": "bb/hand",
            "range": 2.8767
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 2.48,
            "unit": "bb/hand",
            "range": 1.721
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 0.131,
            "unit": "bb/hand",
            "range": 0.8965
          },
          {
            "name": "Results vs. Ourselves",
            "value": -0.8805,
            "unit": "bb/hand",
            "range": 1.3249
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
          "id": "d87f8628a6440e0eb1a4103e71b9bd904b6fa457",
          "message": "final commit",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/54/commits/d87f8628a6440e0eb1a4103e71b9bd904b6fa457"
        },
        "date": 1706756900946,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 3.987,
            "unit": "bb/hand",
            "range": 0.8163
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 53.9935,
            "unit": "bb/hand",
            "range": 2.9207
          },
          {
            "name": "Results vs. Uniform Random Bot",
            "value": 19.012,
            "unit": "bb/hand",
            "range": 2.7387
          },
          {
            "name": "Results vs. Preflop All-in Bot",
            "value": 11.454,
            "unit": "bb/hand",
            "range": 4.4327
          },
          {
            "name": "Results vs. Ourselves",
            "value": -1.467,
            "unit": "bb/hand",
            "range": 3.6211
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
          "id": "da68f294a9a77b85e276056e596fe590848f156d",
          "message": "wip",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/53/commits/da68f294a9a77b85e276056e596fe590848f156d"
        },
        "date": 1706757816404,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 2.7735,
            "unit": "bb/hand",
            "range": 1.2963
          }
        ]
      }
    ]
  }
}
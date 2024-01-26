window.BENCHMARK_DATA = {
  "lastUpdate": 1706236121820,
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
          "id": "f0b573251094dd674241a6c4df73f5b989f918b6",
          "message": "Add hand evaluator that supports 8 cards",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/12/commits/f0b573251094dd674241a6c4df73f5b989f918b6"
        },
        "date": 1706203951402,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.1535,
            "unit": "bb/hand",
            "range": 0.4663
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": 1.8371,
            "unit": "bb/hand",
            "range": 1.4771
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
          "id": "407d56a75b82016ef33074786ed72d03702a69a5",
          "message": "Add hand evaluator that supports 8 cards (#12)\n\n* Add hand evaluator that supports 8 cards\r\n\r\n* Add benchmark script / Remove `constexpr` that requires C++20\r\n\r\n* shuffle\r\n\r\n* rename source file",
          "timestamp": "2024-01-26T02:34:48+09:00",
          "tree_id": "3c0aa3f0f065deddf08861f08cc1fa4b16d37785",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/407d56a75b82016ef33074786ed72d03702a69a5"
        },
        "date": 1706204206358,
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
            "value": -0.2323,
            "unit": "bb/hand",
            "range": 1.4677
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
          "id": "3364c0bda2910f7e32f1f33ce8cde77c2a2804a3",
          "message": "Enforce timeouts in CI/CD",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/16/commits/3364c0bda2910f7e32f1f33ce8cde77c2a2804a3"
        },
        "date": 1706210615682,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.3003,
            "unit": "bb/hand",
            "range": 0.4694
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -1.8118,
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
          "id": "7b49aacf63a5674866f0a5f41a6da42e1be7ae13",
          "message": "Enforce timeouts in CI/CD",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/16/commits/7b49aacf63a5674866f0a5f41a6da42e1be7ae13"
        },
        "date": 1706210958462,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": 0.6092,
            "unit": "bb/hand",
            "range": 0.4763
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -2.4466,
            "unit": "bb/hand",
            "range": 1.4731
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
          "id": "52984c4f1fef6e00e2b4df959f125eb0555366cf",
          "message": "Enforce timeouts in CI/CD (#16)\n\n* test throw on timeouts\r\n\r\n* fix\r\n\r\n* aesthetic change\r\n\r\n* remove fmt/ostream for smaller compile time\r\n\r\n* attempting speedup bot",
          "timestamp": "2024-01-25T14:31:26-05:00",
          "tree_id": "a3077ae5f17000bac55f03605a4514629821c3ff",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/commit/52984c4f1fef6e00e2b4df959f125eb0555366cf"
        },
        "date": 1706211174452,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.4057,
            "unit": "bb/hand",
            "range": 0.4662
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -2.3325,
            "unit": "bb/hand",
            "range": 1.4715
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
          "id": "23db25bfd5c992a982d28d00bf3a5852b4d5c851",
          "message": "Recieve bid",
          "timestamp": "2024-01-24T02:28:03Z",
          "url": "https://github.com/ruse-ai/mitpokerbots-2024/pull/17/commits/23db25bfd5c992a982d28d00bf3a5852b4d5c851"
        },
        "date": 1706214769243,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "Results vs. Check/Call Bot",
            "value": -0.2585,
            "unit": "bb/hand",
            "range": 0.4744
          },
          {
            "name": "Results vs. Bid Everything Bot",
            "value": -0.4415,
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
      }
    ]
  }
}
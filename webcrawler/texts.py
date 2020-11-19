#!/usr/bin/env python3
from asyncio import ensure_future, gather, get_event_loop
from http import HTTPStatus
from json import loads
from sys import stderr
from time import sleep, time
from aiohttp import ClientError, ClientSession

ENCODING = 'UTF-8'
SECS = 15
STEP = 1 << 10
URL = 'https://ru.wikipedia.org/w/api.php'


def get_article_texts(titles: list):
    loop = get_event_loop()
    for start in range(0, len(titles), STEP):
        print(start)
        now = time()
        task = ensure_future(fetch_chunk(titles, start))
        loop.run_until_complete(task)
        result = task.result()
        for i in range(len(result)):
            page = next(iter(loads(result[i])['query']['pages'].values()))
            if 'extract' in page:
                assert titles[start + i] == page['title'], \
                    F"{titles[start + i]} != {page['title']}"
                yield page['title'], page['extract']
            else:
                print(page, file=stderr)
        sleep(max(now + SECS - time(), 0))


async def fetch_chunk(titles: list, start: int) -> list:
    while True:
        try:
            async with ClientSession() as session:
                tasks = []
                for i in range(start, min(start + STEP, len(titles))):
                    tasks.append(ensure_future(fetch(session, titles[i])))
                return await gather(*tasks)
        except (OSError, ClientError) as error:
            print(error, file=stderr)


async def fetch(session: ClientSession, title: str) -> str:
    params = {
        'action': 'query',
        'format': 'json',
        'titles': title,
        'prop': 'extracts',
        'explaintext': 1
    }
    while True:
        async with session.get(URL, params=params) as response:
            if response.status == HTTPStatus.OK:
                return await response.text(encoding=ENCODING)
            print(response, file=stderr)

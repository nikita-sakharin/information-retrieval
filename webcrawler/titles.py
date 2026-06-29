#!/usr/bin/env python3
import dbm
from sys import stderr
from time import time
from wikipediaapi import ExtractFormat, Namespace, Wikipedia, WikipediaPage
from texts import ENCODING

CATEGORIES_FILE = 'categories'
TITLES_FILE = 'titles'

BYTES = int(time()).to_bytes(4, byteorder='big', signed=True)
MAX_BYTES = bytes([255] * 4)
ZERO_BYTES = bytes([0] * 4)
assert(ZERO_BYTES < BYTES and BYTES < MAX_BYTES)


def get_category_titles(category: str):
    page = Wikipedia(
        language='ru',
        extract_format=ExtractFormat.WIKI
    ).page(category)

    kwargs = {'flag': 'cs', 'mode': 0O666}
    with dbm.open(TITLES_FILE, **kwargs) as titles, \
            dbm.open(CATEGORIES_FILE, **kwargs) as categories:
        get_titles(page, titles, categories)

    with dbm.open(TITLES_FILE, flag='r') as titles:
        key = titles.firstkey()
        while key:
            yield key.decode(ENCODING)
            key = titles.nextkey(key)


def get_titles(page: WikipediaPage, titles, categories) -> type(None):
    while True:
        try:
            for member in page.categorymembers.values():
                title = member.title.encode(ENCODING)
                if member.ns == Namespace.MAIN:
                    titles[title] = B''
                elif (member.ns == Namespace.CATEGORY
                        and categories.get(title, ZERO_BYTES) < BYTES):
                    categories[title] = BYTES
                    print(member.title)
                    get_titles(member, titles, categories)
                    categories[title] = MAX_BYTES
                del title
            return
        except OSError as error:
            print(error, file=stderr)

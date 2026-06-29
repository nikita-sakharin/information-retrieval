#!/usr/bin/env python3
from json import dump
from texts import ENCODING, get_article_texts
from titles import get_category_titles


def main(category: str, titles_file: str, texts_file: str, stat_file: str):
    category_titles = sorted(get_category_titles(category))
    with open(titles_file, 'wt', encoding=ENCODING) as file:
        dump(category_titles, file, ensure_ascii=False, separators=(',', ':'))
    """
    with open(titles_file, 'rt', encoding=ENCODING) as file:
        category_titles = load(file)
    """

    with open(texts_file, 'wt', encoding=ENCODING) as file:
        print('{', end='', file=file)
        count, title_size, text_size = 0, 0, 0
        for title, text in get_article_texts(category_titles):
            if count != 0:
                print(',', end='', file=file)
            dump(title, file, ensure_ascii=False)
            print(':', end='', file=file)
            dump(text, file, ensure_ascii=False)
            count += 1
            title_size += len(title.encode(ENCODING))
            text_size += len(text.encode(ENCODING))
        print('}', end='', file=file)

    stat = {'count': count, 'title_size': title_size, 'text_size': text_size}
    with open(stat_file, 'wt', encoding=ENCODING) as file:
        dump(stat, file, indent=4, separators=(',', ': '), sort_keys=True)


if __name__ == '__main__':
    main('Категория:Статьи', 'titles.json', 'texts.json', 'stat.json')

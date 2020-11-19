    with open(texts_file, 'rt', encoding = 'UTF-8') as file:
        title_texts = json.load(file)
    title_size, text_size = 0, 0
    for title, text in title_texts:
        title_size += len(title.encode('UTF-8'))
        text_size += len(text.encode('UTF-8'))
        assert(len(title) > 0)
        assert(len(text) > 0)
    print(F'count = {len(title_texts)}')
    print(F'title_size = {title_size}')
    print(F'text_size = {text_size}')

    with open(texts_file, 'rt', encoding = 'UTF-8') as file:
        index = 240890062
        buffer = file.read()[:index] + '}'
        title_texts = json.loads(buffer)

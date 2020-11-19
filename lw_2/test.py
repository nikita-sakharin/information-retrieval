import json

def main(texts_file: str):
    title_texts = json.loads(read_file(texts_file))

def read_file(filename: str) -> str:
    buffer, step = bytes(), 1 << 31
    with open(filename, 'rb') as file:
        while True:
            temp = file.read(step)
            if not temp:
                break
            buffer += temp
    return buffer.decode('UTF-8')

if __name__ == '__main__':
    main('texts.json')

def replace_tabs_with_spaces(file_path):
    try:
        # Открываем файл на чтение
        with open(file_path, 'r') as file:
            # Читаем строки из файла
            for line in file:
                # Заменяем табуляцию на 3 пробела
                modified_line = line.replace('\t', ' ' * 4)
                # Выводим модифицированную строку в консоль
                print(modified_line, end='')
    except FileNotFoundError:
        print("Файл не найден")


# Пример использования
file_path = './levels/maze.txt'  # Замените на путь к вашему файлу
replace_tabs_with_spaces(file_path)

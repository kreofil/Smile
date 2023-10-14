#! /usr/bin/python3

"""
The MIT License (MIT)

Copyright (c) 2021-2022 c2eo team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

# use: package_compile <path to package>

# import random
import sys
# import time
# import argparse
import subprocess
# import re as regex
import os
from os import chdir
from os import path
from os import listdir
from pathlib import Path
# from shutil import copyfile
# from os import sep as os_sep
from subprocess import CompletedProcess

# Мои модули
import artefact_compile

# Компиляция Всех артефактов, расположенных в одном пакете
def run(path_to_package: Path) -> int:
    # Необходимо проверит, является ли путь к пакету каталогом
    if path.isdir(path_to_package):
        print("path_to_package: " + path_to_package)
    else:
        # Некорректный путь к каталогу пакета
        print(path_to_package + " is not directory")
        return 101
    # Проверка наличия файла экспорта и его создание при отсутствии
    export_file = path_to_package + "/export"
    if not(os.path.exists(export_file)):
        open(export_file, "w").close()
    # Формирование списка артефактов (файлов), расположенных в пакете
    path_to_artefact_directory = path_to_package + "/src/"
    artefacts = listdir(path_to_artefact_directory)
    print(artefacts)
    all_artefacts_count = 0     # Счетчик обработанных артефактов
    err_artefacts_count = 0     # Счетчик артефактов с ошибками
    err_artefacts = []          # Список ошибочных артефактов
    # В цикле запускается компилятор, делающий обход артефактов пакета
    for artefact in artefacts:
        exit_code = artefact_compile.run(path_to_package, artefact)
        # Проверка и предобработка кода возврата
        # print(exit_code)
        all_artefacts_count += 1
        if exit_code != 0:
            err_artefacts_count += 1
            err_artefacts.append(artefact)
    # Информация о результатах обработки пакета
    print('Numer of artefacts: ', all_artefacts_count)
    print('Numer of errors in artefacts: ', err_artefacts_count)
    print('Errors in artefacts: ', err_artefacts)
    # Возврат числа ошибок в пакете
    return err_artefacts_count        # TODO

# Тестовый запуск для одного пакета
if __name__ == '__main__':
    assert sys.version_info >= (3, 10)
    path_to_package = path.abspath(sys.argv[1])
    return_code = run(path_to_package)
    # print(return_code)
    # Сформированный код возврата
    exit(return_code)


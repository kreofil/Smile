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
# import random
import sys
# import time
# import argparse
import subprocess
# import re as regex
from os import chdir
from pathlib import Path
# from shutil import copyfile
# from os import sep as os_sep
from os import path
from subprocess import CompletedProcess

# Компиляция одного артефакта из пакета
def run(path_to_package: Path, artefact: str) -> int:
    # Необходимо проверить, является ли путь к пакету каталогом
    if path.isdir(path_to_package):
        print("path_to_package: " + path_to_package)
    else:
        # Некорректный путь к каталогу пакета
        print(path_to_package + " is not directory")
        return 101
    # Формирование пути к артефакту
    path_to_artefact = path_to_package + "/src/" + artefact
    # print("path_to_artefact: " + path_to_artefact)
    if path.isfile(path_to_artefact):
        # Артефакт доступен
        # print("artefact:" + path_to_artefact)
        # Формируется командная стока для запуска компилятора артефатов
        command_string = ["../bin/smile","-p",path_to_package,"-e",artefact]
        # Запуск компилятора артефактов
        exit_code = subprocess.run(command_string)
        # print(exit_code)
        return exit_code.returncode
    else:
        # Артефакт недоступен
        # print(path_to_artefact + " is not artefact")
        exit(102)

# Тестовый запуск для одного артефакта
if __name__ == '__main__':
    assert sys.version_info >= (3, 10)
    path_to_package = path.abspath(sys.argv[1])
    artefact = sys.argv[2]
    return_code = run(path_to_package, artefact)
    # print(return_code)
    # Сформированный код возврата
    exit(return_code)


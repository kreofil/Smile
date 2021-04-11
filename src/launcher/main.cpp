#include <boost/program_options.hpp>
using namespace boost::program_options;

#include <string>
#include <vector>
//#include <algorithm>
//#include <iterator>
#include <iostream>
#include <fstream>

#include "smodel.h"


// Создатель семантической модели
class SemanticModel;
SemanticModel* GetSemanticModel();
// Экспорт функции запуска компилятора файла экспорта
bool ExportCompile(std::vector<std::string> &e, SemanticModel* m);
// Экспорт функции запуска компилятора артефактов
bool Compile(std::string &a, SemanticModel* m);

void to_cout(const std::vector<std::string> &v)
{
  std::copy(v.begin(), v.end(),
    std::ostream_iterator<std::string>{std::cout, "\n"});
}

int main(int argc, const char *argv[])
{
    // Распознавание опций командной строки, обеспечивающих выбор
    // каталога проекта и компилируемой сущности (артефакта)
    std::string packageDir;     // каталог пакета содержащего компилируемую сущность
    std::string entityName;         // компилируемая сущность

    try {
        options_description desc{"Options"};
        desc.add_options()
            ("help,h", "Help screen")
            ("package,p", value(&packageDir), "Package")
/*            ("entity,e", value<std::vector<std::string>>()->
                multitoken()->zero_tokens()->composing(), "Compiled entity");
*/
            ("entity,e", value(&entityName), "Compiled entity");

        // Ограничение позиционных аргументов одним (возможны без опции)
        positional_options_description pos_desc;
        //pos_desc.add("phone", -1);
        pos_desc.add("entity", 1);

        command_line_parser parser{argc, argv};
        parser.options(desc).positional(pos_desc).allow_unregistered();
        parsed_options parsed_options = parser.run();

        variables_map vm;
        store(parsed_options, vm);
        notify(vm);

        // Вывод подсказки
        if (vm.count("help")) {
            if(argc > 2) {
                std::cout << "Use only one option --help or -h for help\n";
                return 0;
            }
            std::cout << desc << '\n';
            return 0;
        }
    
        // Вывод непозиционных параметров (в нашем случае он один
        if (vm.count("entity")) {
            /*
            to_cout(vm["entity"].as<std::vector<std::string>>());
            std::cout << "Entity is: " << vm["entity"].as<std::vector<std::string>>()[0] << "\n";
            */
            std::cout << "Entity is: " << entityName << "\n";
        }

        if (vm.count("package")) {
            std::cout << "Package is: " << packageDir << '\n';
            //return 0;
        }
    }
    catch (const error &ex) {
        std::cerr << ex.what() << '\n';
    }
    
    // После получения имен открытие и чтение файлов экспорта и артефакта
    // с последующей передачей их на обработку.
    std::string exportName{packageDir + "/export"};
    std::string artefactName{packageDir + "/src/" + entityName};
    std::string rigName{packageDir + "/rig/" + entityName};
    
    // Компилируемый артефакт читается из файла в строку
    std::ifstream artefactStream(artefactName);
    if(artefactStream.fail()) {
        std::cout << "Artefact" << artefactName << " isn't accesible" << std::endl;
        return 1;
    }
    std::stringstream ss;
    ss << artefactStream.rdbuf();
    std::string artefact(ss.str());
    /// Тестовый вывод прочитанного артефакта
    std::cout << artefact << std::endl;
    // Файл можно закрыть
    artefactStream.close();

    // Файл экспорта читается в список строк
    std::ifstream exportStream(exportName);
    if(exportStream.fail()) {
        std::cout << "Export" << exportName << " isn't accesible" << std::endl;
        return 2;
    }
    std::string str;
    std::vector<std::string> exportList{};
    while (getline(exportStream, str)) {
        exportList.push_back(str);
    }
    // Файл можно закрыть
    exportStream.close();
    /// Тестовый вывод списка экспорта
    //for(auto s: exportList) {
    //    std::cout << s << "\n";
    //}
    
    SemanticModel *sm = GetSemanticModel();
    if(!ExportCompile(exportList, sm)) {
        std::cout << "Error in Export" << std::endl;
        return 3;
    }
    if(Compile(artefact, sm)) {
        // Повторное открытие файла экспорта для записи результата компиляции артефакта
        // и перезаписи экспортируемых объявлений
        std::ofstream toExportStream(exportName);
        if(toExportStream.fail()) {
            std::cout << "Export for writing" << exportName << " isn't accesible" << std::endl;
            return 3;
        }
        sm->Store(toExportStream);
        toExportStream.close();
    }
    
    return 0;
}

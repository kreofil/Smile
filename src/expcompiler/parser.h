#ifndef __data__
#define __data__

#include <string>
#include <vector>

//#include "../smodel/smodel.h"
#include "smodel.h"

class ExportCompiler {
    std::string currentDcl;    // ссылка на строку с текущим объявлением
    //std::vector<std::string> &exported;   // экспортируемые данные
    int line;                   // строка (индекс) обрабатываемого объявления
    int column;                 // позиция в строке для обрабатываемого объявления
    std::string lexValue;            // подстрока, определяющая текущую набранную лексему
    int errCount;       // счетчик ошибок (теоретически их быть не должно)
    char symbol;        // текущий символ
    int oldPos;
    int oldLine;
    int oldColumn;
    char oldSymbol;
    int qualCount;      // Счетчик идентификаторов в составном имени
    static std::vector<std::string> keyWords;   // таблица ключевых слов, используемых в экспорте
    SemanticModel &sm;  // Семантическая модель языка
public:

    //explicit ExportCompiler(std::vector<std::string> &e);
    explicit ExportCompiler(SemanticModel &m);

    //=============================================================================
    // Весь экспорт как набор анализируемых строк объявлений
    //=============================================================================
    // Установка строки для обработки
    void setLine(std::string s, int l) {
        currentDcl = s;
        line = l;
        column = 0;
        symbol = currentDcl[0];
    }
    bool isExport();

private:
    //=============================================================================
    // Методы парсера экспорта
    //=============================================================================
    // Экспорт константы
    bool isConstExport(Declaration** ppdcl);
    // Экспорт функции
    bool isFuncExport(Declaration** ppdcl);
    // Экспорт типа.
    bool isTypeExport(Declaration** ppdcl);
    // Экспорт переименованного типа.
    bool isBaseExport(Type** pptv);
    // Экспорт кортежа
    bool isTupleExport(Type** pptv);
    // Экспорт структуры
    bool isStructExport(Type** pptv);
    // Экспорт импорта.
    bool isImportExport();
    
    
    //=============================================================================
    // Методы сканера
    //=============================================================================
    // Идентификатор. Не должен быть ключевым словом
    bool isId();
    // Проверка, что это одно из ключевых слов
    bool isOneOfReservedWord(std::string v);
    // Простое имя, являющееся идентификатором без дополнительных полей
    bool isSimpleName();
    // Квалифицированный идентификатор (идентификаторы, разделенные точками)
    bool isQualId();
    // Зарезервированное слово
    bool isReservedWord(std::string &&v);
    // Комментарий
    bool isComment();
    // Однострочный комментарий.
    bool isOneStringComment();
    // Многострочный комментарий.
    bool isManyStringComment();
    // Левое обозначение
    bool isLeftAssign();
    // Правое обозначение
    bool isRightAssign();
    // Разделитель аргумента и результата
    bool isArrow();
    // Булева константа true | false
    bool isBoolean();
    // Целочисленная константа со знаком или без знака
    bool isInteger();
    // Действительная константа со знаком или без знака
    bool isFloat();
    // Символьная константа
    bool isChar();
    
    //=============================================================================
    // Методы транслитератора
    //=============================================================================
    // Проверка символа на латинскую букву
    bool isLetter(char ch);
    // Проверка символа на цифру
    bool isDigit(char ch);
    // Проверка символа на пустой разделитель (пробел, табуляция...)
    bool isSkip(char ch);
    // Проверка символа на пустой разделитель без перевода строки
    bool isSpace(char ch);
    // Проверка символа на перевод строки
    bool isLf(char ch);
    // Проверка символа на ';' или перевод строки (для анализа разделителя)
    bool isLfOrSemicoln(char ch);
    // Проверка символа на конкретное значение
    bool isSymbol(char ch);
    // Метод, проверяющий достижение конца разбираемой строки
    bool isEnd();
    // Символ, возможный после слэша
    bool isAfterSlashSymbol(std::string &v);
    // Символ, недопустимый без слэша
    bool isUsingSlashSymbol();
    
    //=============================================================================
    // Вспомогательные методы
    //=============================================================================
    // Читает следующий символ из входной строки
    void nextSym();
    // Сохранение текущей позиции для возврата назад
    void storePos();
    // Восстановление позиции после отката
    void restorePos();
    // Пропуск пробельных символов и комментариев
    void Ignore();
    // Выдача сообщения об ошибке
    void Err(std::string &&s );

};

// Начальные установки параметров компилятора и его запуск
void Compile(const std::string &a, std::vector<std::string> &e);

#endif

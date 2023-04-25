#ifndef __parser__
#define __parser__

#include <string>
#include <vector>

//#include "../smodel/smodel.h"
#include "smodel.h"

class Compiler {
    std::string &artefact;      // ссылка на строку с артефактом
    //---std::vector<std::string> &exported;   // экспортируемые данные
    int pos;                    // текущая позиция в тексте обрабатываемого артефакта
    int line;                   // строка обрабатываемого артефакта
    int column;                 // колонка в строке для обрабатываемого артефакта
    std::string lexValue;       // подстрока, определяющая текущую набранную лексему
    int errCount;               // счетчик ошибок
    char symbol;                // текущий символ
    int oldPos;
    int oldLine;
    int oldColumn;
    char oldSymbol;
    int qualCount;              // Счетчик идентификаторов в составном имени
    static std::vector<std::string> keyWords;   // таблица ключевых слов
    SemanticModel &sm;          // Семантическая модель языка
public:

    explicit Compiler(std::string &a, SemanticModel &m);

    //=============================================================================
    // Начальный нетерминал
    //=============================================================================
    //! Artefact = ConstDefinition | FuncDefinition | ProtoDefinition
    //!          | TypeDefinition  | ImportDefinition
    bool isArtefact();

private:
    //=============================================================================
    // Методы парсера
    //=============================================================================
    // Определение константы
    bool isConstDefinition(Declaration** ppdcl);
    // Определение функции
    bool isFuncDefinition(Declaration** ppdcl, Func** ppf);
    // Тело функции
    bool isBody(Func& f);
    // Элемент тела функции
    bool isElement(Func& f);
    // Выражение в теле функции
    bool isExpression(Func& f, Actor** ppa);
    // Префиксное подвыражение в теле функции
    bool isPrefixExpression(Func& f);
    // Терм
    bool isTerm(Func& f, Actor** ppa);
    // Определение типа.
    bool isTypeDefinition(Declaration** ppdcl);
    // Определение базового типа.
    bool isBaseType(Type** pptv);
    // Определение вектора.
    bool isArray(Type** pptv);
    // Определение именованной структуры.
    bool isStruct(Type **pptv);
    // Определение неименованной структуры.
    bool isTuple(Type **pptv);
    // Определение роя.
    bool isSwarm(Type** pptv);
    // Определение импорта.
    bool isImportDefinition();
    // Определение константного выражения.
    bool isConstExpression(Const** ppcv);
    // Константный терм, содержащий константы и константные операции
    bool isConstTerm(Const** ppcv);
    // Атомарная константа
    bool isAtom(Const** ppcv);
    // Константный вектор (список данных)
    bool isConstDataList();
    // Константная группа (параллельный список)
    bool isConstParList();
    // Строка символов
    bool isString();
    
    
    
    //=============================================================================
    // Методы сканера
    //=============================================================================
    // Идентификатор. Не должен быть ключевым словом
    bool isId();
    // Проверка, что это одно из ключевых слов
    bool isOneOfReservedWord(std::string &v);
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
    // Одна из базовых функций, задаваемых специальным знаком или их набором
    bool isBaseFunc(DeclarationFunc** ppdf);
    
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

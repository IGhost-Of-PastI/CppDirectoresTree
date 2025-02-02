#include <iostream>  
#include <filesystem>
//#include "DirectoresTree.h"
#include <Windows.h>
#include <memory>
#include <thread>
#include <format>
#include <functional>
#include <efsw/efsw.hpp>
#include "TestDirectoryWatcher.h"

using namespace std;

std::string thisThreadIdToString() {
    std::ostringstream oss;
    oss << this_thread::get_id(); // Преобразование id в строку  
    return oss.str();
}

int main() {
    using namespace std::filesystem;
    using namespace std;
    using namespace efsw;
    
    setlocale(LC_ALL, "");
    //DirectoriesTree<int> dirTree=current_path().parent_path().append("x64").append("Debug");

    //auto elm = dirTree[current_path().parent_path().append("x64").append("Debug").append("TestFolder").append("InnerFolder1")  ];

    //cout << dirTree.GetRootPath() << endl;

    //_rootNode = std::make_shared<NodeType>(rootDir);

    const path currentFolder = current_path().parent_path()/"x64"/"Debug";
    string formatedString = format("Главный поток процесса: {}", thisThreadIdToString());
    cout << formatedString << endl;

    std::function<void(string, string, string)> onModifed = [](string dir, string filename, string oldname) {
        cout << format("Поток исполнения обработки: {}, Данные изменений: {} / {} >> {}", thisThreadIdToString(),path(dir),filename,oldname) << endl;
    };
    std::function<void(string, string, string)> onAded = [](string dir, string filename, string oldname) {
        cout << format("Поток исполнения обработки: {}, Данные добавления: {} / {} >> {}", thisThreadIdToString(), path(dir), filename, oldname) << endl;
        };
    std::function<void(string, string, string)> onDeleted = [](string dir, string filename, string oldname) {
        cout << format("Поток исполнения обработки: {}, Данные удаления: {} / {} >> {}", thisThreadIdToString(), path(dir), filename, oldname) << endl;
        };
    std::function<void(string, string, string)> onMoved = [](string dir, string filename, string oldname) {
        cout << format("Поток исполнения обработки: {}, Данные перемещения: {} / {} >> {}", thisThreadIdToString(), path(dir), filename, oldname) << endl;
        };

    const unique_ptr<FileWatcher> fileWatcher = make_unique<FileWatcher>();
    const unique_ptr<TestDirectoryListener> directoryListener = make_unique<TestDirectoryListener>(onAded,onDeleted,onModifed,onMoved);
    WatcherOption option(efsw::Options::Option::WinBufferSize, 64 * 1024);
    WatchID watchID = fileWatcher->addWatch(currentFolder.string(), directoryListener.get(), true, {option});
    fileWatcher->watch();

    bool isExecuted = false;
    while (true)
    {
        if (!isExecuted)
        {
            create_directory(path(currentFolder).append("new"));
            cout << "Директория new создана" << endl;
            create_directory(currentFolder / "Новая");
            cout << "Директория Новая создана" << endl;
            rename(currentFolder / "new", currentFolder / "new2");
            cout << "Директория new переимнована в new2" << endl;
            rename(currentFolder / "Новая", currentFolder / "Новая2");
            cout << "Директория Новая переимнована в Новая2" << endl;
            remove_all(currentFolder/"new2");
            cout << "Директория new2 удалена" << endl;
            remove_all(currentFolder / "Новая2");
            cout << "Директория Новая2 удалена" << endl;
            //cout << "Директория new создана" << endl;
            //ююrename(path(currentFolder).append("new"), path(currentFolder).append("old"));
            //ююcout << "Директория new переимнована в old" << endl;
            isExecuted = true;
        }
        
    }
    //Провести тесты с русскими названиями папок чтобы они были синхронизированы у вотчера и filesystem, вместо использования пути как строки используй путь файл система
    //Замечания по тестам
    /*
    У директории нет понятия модифайд у него есть понятие перемещения
    Пока по наблюдениям наблюдать всегда видит изменения позже его применения но это не значит что обработчик будет вызван до того как информация о изменении посутп вотчеру
    следвоательно в вотчера должен быть мультимап изменений перед внесением изменений к нему необходимо блокировать доступ, и при изменении записывать утда даныне и особождать, так гаранитровано алогритм ихзменений в директориях успеет оповестить наблюдателя об этом до того как он увидит изменение
    */


    
    //TO DO
    /*
   -- 1. Изменить сам класс ноды и его шаблон нет никакого смысла в применении дефолтныхз значений поскольку у классво и так в большинстве своём есть консткруторы по умолчанию
    2. Класс ноды должен обладать блокировками в методах связаных с пересозданием каталогов.
    3. Класс ноды должен обновляться а не пересоздаваться, поскольку directory_entry поддерживает реврешь, нужно лишь узнать поддерживате ли он рефреш при иззменении родительского каталога
    4. Класс наблюдатель, явно работает в другом потоке (проверить через this_thread.id) по этой причине ему нужно обеспечить безопасное обновление каталогов, также поскольку само приложение может менять каталоги
    то требуеться очередь где приложение вносит всё что оно сделало а при обработке изменений вотчер смотрит нет ли связаных с данным  типом изменения и с данным файлом (мульт индекс) (следует выяснить ктаже порядок вызова изменения и наблюдателя чтобы понятьк ак записывать адныне в очередь)
    5. Нода может быть возвращена из дерева но она обязательно должна быть указателем на константыне данные.
    6. (древовидная структура не труебуеться но относиться к ней) Требуеться иметь поиск именно файлов в дереве директорий, поиск можно осущестлветяь через recursive_directory итератор а потом по нужным файлам брать их родительский каталог, и упаковывать это все в список резхультатов поиска. 
    Примечение: естсвтенно могут попадаться одинаковые файлы, для игры это не так критично поскльку в файлах будут содержаться разнообразные шаблоны компонетов или объектов, они не являютсья лкассами а инсутркциями по их созданию, и они уже обладают уникальным путём.
    */
    return 0;
}
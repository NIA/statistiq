StatistiQ — статистика эскспериментальных данных
================================================

![Главное окно StatistiQ][statistiq-screen] ![Отчёт StatistiQ][statistiq-report-screen]

Это простое Qt-приложение, которое позволяет вводить численные данные
из эксперимента или считывать их из файла и затем рассчитывает и
показывает различную статистическую информаци об этих данных.

Основные функции
----------------

- Ввод данных с клавиатуры или из файла
- Редактирование данных (правка, добавление, удаление)
- Расчёт среднего, СКО, дисперсии и моментов.
- Построение настраиваемой гистограммы и кривой данных
- Показ отчёта с возможностью сохранить его в html или напечатать

Компиляция
----------

Чтобы собрать StatistiQ из исходного кода, потребуются установленные [Qt SDK] и [Qwt 6.0.1].

Откройте `statistiq.pro` в Qt Creator и нажмите *Запустить*.

Или, из командной строки, зайдите в папку, где находится `statistiq.pro`, и выполните

    qmake statistiq.pro
    make release

Чтобы построить переводы для русского языка, также выполните

    lrelease statistiq.pro


  [Qt SDK]: http://qt-project.org/downloads
  [Qwt 6.0.1]: http://sourceforge.net/projects/qwt/files/qwt/6.0.1
  [statistiq-screen]: https://dl.dropbox.com/u/6068922/statistiq-screen.png
  [statistiq-report-screen]: https://dl.dropbox.com/u/6068922/statistiq-report-screen.png


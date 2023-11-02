# 36.7. Итоговый проект клиентская часть...
  
    Изменения и дополнения к интерфейсу клиентской части приложения (от 0 до 10 баллов):
    Удобство интерфейса (5 балла)
    Дизайн интерфейса (5 балла)
    Удобство приложения (от 0 до 10 баллов):
    Функциональность и работоспособность (5 балла)
    Производительность и стабильность работы (5 балла)

## Реализованно:
      Реализован UI для чата с возможностью писать публичные и личные сообщения
      При входе необходимо ввести данные для авторизации либо регистрации (2 формы)
      Само приложение содержит 2 подэкрана для отображения пользователей
      и отображения сообщений
      При нажатиии на пользователя отображаются сообщения между авторизованным пользователем и выбранным (приватный чат)
      Выход из приватного чата производится нажатием на кнопку -> вернутся в публичный чат

  ### Установка:
      скачать проект,
      создать каталог 'Chat' рядом с проектом,
      перейти в этот каталог,
      выполнить:  cmake 'путь до каталога с проектом'
      выполлнить: make
      в каталоге 'Chat' будет находится исполняемый файл ChatServer

      В случае если при старте приложения будет предупреждение что не загрузились файлы перевода,
      то поместите переводы из папки 'translations' в папку 'Chat_translations' по пути из сообщения!

### Требования:  
    Сначало необходимо запустить сервер!!!
    После успешного подключения программа создаст дефолтные таблицы БД


### Компиляция



https://github.com/ivanKispaj/Client_QT/assets/91827767/83011372-1603-4a59-abb9-b07f1bcdb65f




### Демонстрация работы



https://github.com/ivanKispaj/Client_QT/assets/91827767/e2fff173-c2b2-4496-98d6-43a98d42e0d5




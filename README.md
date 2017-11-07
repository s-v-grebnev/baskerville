# BASKERVILLE: Задача для секции

### Постановка задачи

Необходимо разработать клиент-серверное приложение, реализующее защищенную
пересылку файлов.

На сервере можно задать несколько корзин, куда будут приниматься и складываться
 файлы от клиентов после проверки ЭП. Корзина имеет уникальный идентификатор
 (число-буквенный), маппится в конкретный каталог на файловой системе, заданный
  в конфиг-файле. Клиент может отправить файл с использованием клиентской
  утилиты, принимающей в качестве параметров: адрес сервера, имя файла
  и закрытый ключ для подписи файла.

Сервер: стартует, читает конфигурацию и ждет клиентов.
При получении команды листания корзины - отправляет список имен файлов из нее.
При получении команды на выкладку файла - получает файл, проверяет ЭП,
выкладывает в корзину если подпись верна.

Клиент реализует режимы листания и отправки. В режиме листания можно вызвать
cli-утилиту с параметром "идентификатор корзины" и получить с сервера список
файлов из нее. В режиме отправки можно указать имя файла, идентификатор корзины
и ключ для формирования подписи. Файл будет подписан и отправлен на сервер.

Сервер позволяет настроить через конфиг:
Каталог для размещения корзин
ID корзин для приема сообщений
Файл с публичным ключом или сертификатом для проверки подписи файлов

Приложение не должно использовать сторонние сервисы.
Протокол взаимодействия клиента с сервером - любой. Например, gRPC. Язык - С++.
Обработку ошибок в некоторых случаях можно ограничить, но в комментариях
 желательно написать об этом.

## Решение

## Сервер

Сервер читает конфиг из файла **config.txt**.

В файле можно прописать:
* Номер порта:

**Port=10050**

* Путь к каталогу с корзинами:

**Path=/tmp/baskets**

* Путь к файлу с открытым ключом RSA в формате PEM:

**Pubkey=public.key.pem**

* Несколько корзин:

**Basket=asdf1234**

**Basket=sdfg2345**

**Basket=qwertyui**

Ключ и значение разделяются знаком "=" без пробелов.

Сервер ничего не выводит в консоль, за исключением диагностики сообщений
об ошибках.
По умолчанию сервер слушает порт 10050.

## Клиент

Клиент -- это cli-программа.

Параметры для запуска в режиме отправки:

**-s -h host:port -f filename -i basketid -k key**

Параметры для запуска в режиме листания:

**-b -h host:port -i basketid**

Опции имеют и длинную форму.

* -h, --host host:port : хост и порт сервера

* -s, --send : режим отправки

* -b, --browse : режим листания

* -i, --basketid : идентификатор корзины

* -f, --filename filename : имя файла для отправки

* -k, --key keyfile.pem : имя файла с секретным ключом RSA в формате PEM.

В режиме отправки клиент сообщает о результате отправки, в режиме листания
выводит содержимое корзины.
При ошибке в опциях выводится напоминание о порядке вызова. Параметры
по умолчанию не заданы.

## Сборка

Для сборки нужны: openssl, grpc++, protobuf.
Сборка при помощи отдельного **Makefile**.

**make all** собирает проект.

**make install** копирует клиент **client** и сервер **server**
 в подкаталог **bin/**

## Внутреннее устройство

Общение клиент-сервер осуществляется через gRPC. Описание протокола такое:

```
syntax = "proto3";

package BasketApi;

message BasketListRequest{
	string basketid = 1;
}

message BasketListResponse{
	repeated string filenames = 1;
}

message BasketPutFileRequest{
	string basketid = 1;
	string filename = 2;
	bytes content = 3;
	string signature = 4;
}

message BasketPutFileResponse{
	string success = 1;
}

service BaskApi{
	rpc BasketList(BasketListRequest) returns (BasketListResponse) {}
	rpc BasketPutFile(BasketPutFileRequest) returns (BasketPutFileResponse) {}
}
```
Для цифровой подписи применяется алгоритм RSA+SHA512, реализованный в OpenSSL.
Ключи хранятся в файлах формата PEM. Подпись передается как
base64-кодированная строка.

Для подписи реализованы два класса-криптопровайдера (см. **rsa.hpp**):
_RSASignProvider_, _RSAVerifyProvider_ с очевидным функционалом. Методы классов
включают загрузку ключа из PEM-файла, выработку или проверку подписи в бинарном
и base64-кодированном форматах.
Работа с base64 вынесена во вспомогательный файл **base64.hpp**.

Опции командной строки клиента разбирает при помощи стандартной функции
_getopt_long()_
метод _ClientOptions::ParseOptions_ (**coptions.hpp**). Конфиг-файл сервера
 парсится
методом _ServerOptions::ParseFile_ (**soptions.hpp**).

Операции с файлами и каталогами на стороне сервера реализованы в классе
_FileOperator_
(**serverfileop.hpp**). Размер передаваемых файлов ограничен использованными
методами gRPC (примерно 4 MB).

Кухня методов gRPC  находится в файлах
**basket.grpc.pb.cc**,  **basket.grpc.pb.h**, **basket.pb.cc**,
 **basket.pb.h**, сгенерированных компилятором **protoc** из описания,
 приведенного выше.

В файле **client.cpp** реализован класс _BasketClient_, который отвечает
 за клиентскую часть протокола,
а также вспомогательная функция _ReadFileContents_.

В файле **server.cpp** реализован класс  _BasketServer_, который отвечает
за серверную часть протокола.

## Обрабатываемые ошибки

Обрабатываются ошибки чтения-записи файлов, операций с цифровой подписью,
разбора опций командной строки и конфиг-файлов. Ошибки gRPC обрабатываются
на стороне клиента.

## Пример работы.

Нужно подготовить ключи подписи и проверки: взять готовые файлы из архива
или выполнить, например:

**ssh-keygen -t rsa -f key**

**openssl rsa -in key -out secret.key.pem**

**openssl rsa -in key -pubout -out public.key.pem**

Подготовить файл **config.txt**, прописав в него путь до каталога с корзинами
(каталог должен существовать), путь к файлу с ключом подписи **public.key.pem**,
несколько идентификаторов корзин (подкаталоги-корзины будут созданы при первом
к ним обращении). Опционально можно указать номер порта.

Теперь можно запустить **server** (**config.txt** должен быть в том же
каталоге).

Далее можно запускать **client** в различных режимах, опции перечислены выше.
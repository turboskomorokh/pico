# pico
Пакетный менеджер, обрабатывающий пакет вида  
```
package.tar
|_ pico-package
\_ [ содержимое ]
```
Составляет список содержимого и устанавливает содержимое в корень.

pico-package имеет вид:
```
name: package_name
version: package_version
arch: package_arch
depends: package1 package2
conflicts: package0 package3
```

Проблемы:
 * Установка одного пакета за раз

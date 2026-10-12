## Вмикання підтримки генерації flatpak пакету

> **Спершу прочитай це.** Flatpak пакує *застосунок* - він має ідентифікатор застосунку, команду `command` для запуску і пісочницю, у якій його запускати. Даний проект будує динамічну бібліотеку і взагалі жодного виконуваного файлу, отож запускати flatpak-у тут нічого. Відповідно ціль flatpak **більше не під'єднана до системи побудови** даного проекту: модуль [cmake/enablers/packagers/template-project-flatpak-target.cmake](/cmake/enablers/packagers/template-project-flatpak-target.cmake) досі присутній у дереві проекту, успадкований від шаблону застосунку, з якого виріс даний проект, але жоден `include()` його не підтягує, отож `-DENABLE_FLATPAK=ON` наразі не має жодного ефекту. Секція збережена заради тих, хто захоче повернути ціль назад для застосунку, побудованого поверх даної бібліотеки. Для розповсюдження самої бібліотеки використовуй пакувальники [DEB](/doc/sections/uk_UA/5-project-build/packagers/5-10-enabling-DEB-package-generation-with-cpack.md), [RPM](/doc/sections/uk_UA/5-project-build/packagers/5-22-enabling-RPM-package-generation-with-cpack.md), [FreeBSD pkg](/doc/sections/uk_UA/5-project-build/packagers/5-20-enabling-FreeBSD-pkg-package-generation-with-cpack.md) чи [WIX MSI](/doc/sections/uk_UA/5-project-build/packagers/5-21-enabling-WIX-MSI-package-generation-with-cpack.md) - усі вони під'єднані і працюють.

Якщо ж модуль буде повернуто до [cmake/template-project-feature-enabler-post.cmake](/cmake/template-project-feature-enabler-post.cmake), то генерація пакету постачання flatpak вмикається конфігуруванням проекту з параметром `ENABLE_FLATPAK` (для GNU/Linux подібних ОС):

```
# всередині кореневої директорії проекту 

mkdir -vp build && cd build && cmake ../ -DENABLE_FLATPAK=ON
```

Успішно виконана команда у свою чергу увімкне ціль для побудови під назвою `flatpak`.

На кінець, щоб згенерувати пакет розповсюдження flatpak необхідно побудувати ціль `flatpak`:

```
# всередині директорії побудови проекту

cmake --build . --target flatpak
```

У разі успішної побудови цілі буде створено файл під назвою, наприклад, `ImagesAnnotatorDataDrivers-0.13.0.flatpak` у кореневій директорії побудови проекту. Для того щоб преглянути або/і змінити ціль побудови пакету flatpak необхідно відвідати файли [cmake/enablers/packagers/template-project-flatpak-target.cmake](/cmake/enablers/packagers/template-project-flatpak-target.cmake) або [misc/packagers/flatpak.conf.json.in](/misc/packagers/flatpak.conf.json.in).

Зауваж, що і назва файлу пакунку, і ідентифікатор застосунку складаються зі змінної `PROJECT_BINARY_NAME` - назви виконуваного файлу проекту, якої даний проект більше не оголошує. Шаблоном ідентифікатора є `ua.org.kytok.template.${PROJECT_BINARY_NAME}`, отож перш ніж ціль зможе створити щось осмислене, ту змінну (і команду `command` для запуску у пісочниці) має надати той, хто повертатиме ціль назад.

Необхідно звернутися до документації програми flatpak для того щоб дізнитись як встановлювати або запускати програми котрі розповсюджуються у вигляді пакету flatpak. З визначеною назвою команди встановлення і запуску виглядали б наступним чином:

```
# для встановлення (версія може змінитись)
# всередині директорії побудови проекту 
flatpak install --user ImagesAnnotatorDataDrivers-0.13.0.flatpak

# для запуску встановленої програми
flatpak run ua.org.kytok.template.ImagesAnnotatorDataDrivers
```

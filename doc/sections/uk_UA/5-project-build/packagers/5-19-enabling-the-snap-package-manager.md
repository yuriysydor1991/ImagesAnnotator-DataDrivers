## Вмикання пакувальника snap

> **Спершу прочитай це.** Так само як і flatpak, [SNAP](https://snapcraft.io/docs/) пакує *застосунок*: файл `snapcraft.yaml`, з якого він будується, оголошує секцію `apps:` з командою `command` для запуску. Даний проект будує динамічну бібліотеку і взагалі жодного виконуваного файлу, отож запускати snap-у тут нічого. Тому ціль snap **більше не під'єднана до системи побудови** даного проекту - модуль [cmake/enablers/packagers/template-project-snap-enabler.cmake](/cmake/enablers/packagers/template-project-snap-enabler.cmake) досі присутній у дереві проекту, успадкований від шаблону застосунку, з якого виріс даний проект, але жоден `include()` його не підтягує, отож `-DENABLE_SNAP=ON` наразі не має жодного ефекту. Для розповсюдження самої бібліотеки використовуй пакувальники [DEB](/doc/sections/uk_UA/5-project-build/packagers/5-10-enabling-DEB-package-generation-with-cpack.md), [RPM](/doc/sections/uk_UA/5-project-build/packagers/5-22-enabling-RPM-package-generation-with-cpack.md), [FreeBSD pkg](/doc/sections/uk_UA/5-project-build/packagers/5-20-enabling-FreeBSD-pkg-package-generation-with-cpack.md) чи [WIX MSI](/doc/sections/uk_UA/5-project-build/packagers/5-21-enabling-WIX-MSI-package-generation-with-cpack.md) - усі вони під'єднані і працюють.

Якщо ж модуль буде повернуто до [cmake/template-project-feature-enabler-post.cmake](/cmake/template-project-feature-enabler-post.cmake), то ціль `snap`, що призначена для генерування SNAP бінарного дистрибуційного пакунку проекту, вмикається конфігуруванням проекту зі змінною CMake `ENABLE_SNAP`:

```
cmake -S . -B build -DENABLE_SNAP=ON
```

Ціль конфігурує шаблон [misc/packagers/snapcraft.yaml.in](/misc/packagers/snapcraft.yaml.in) у `<директорія побудови>/snap/snapcraft.yaml` і потім виконує над ним `snapcraft pack`, що створило б пакунок, названий за полями `name` і `version` snap-а, наприклад `imagesannotatordatadrivers_0.11.0_amd64.snap`.

Дві кеш-змінні налаштовують метадані: `TEMPLATE_PROJECT_SNAP_GRADE` (за замовчуванням `devel`) і `TEMPLATE_PROJECT_SNAP_CONFINEMENT` (за замовчуванням `strict`).

Зауваж, що `name` snap-а, запис `apps:` і його `command` складаються зі змінної `PROJECT_BINARY_NAME` - назви виконуваного файлу проекту, якої даний проект більше не оголошує. Ту змінну має надати той, хто повертатиме ціль назад, інакше згенерований `snapcraft.yaml` описуватиме застосунок, якого не існує.

Детальніше у файлі [cmake/enablers/packagers/template-project-snap-enabler.cmake](/cmake/enablers/packagers/template-project-snap-enabler.cmake) про ціль `snap`.

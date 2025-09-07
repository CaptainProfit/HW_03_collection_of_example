cat print не работает, иногда падает с сегфолтом и блокирует работу rmmod/insmod
Вся работа ведется из папки `hw3/ex_list`
## сборка
в папке с модулем  выполнить `make`
 ## тест 1
 проверяет добавление в/удаление из начала списка.
  **Ожидаемый результат:**
```
[ 1493.896023] ex_list: loaded
[ 1493.906280] ex_list: added 1
[ 1493.908534] ex_list: added 2
[ 1493.910679] ex_list: added 3
[ 1493.912960] ex_list: added 4
[ 1493.913620] ex_list: {[], 4, 3, 2, 1}
[ 1493.913687] ex_list: pointer on 1
[ 1493.919352] ex_list: removed 1
[ 1493.922029] ex_list: removed 2
[ 1493.922830] ex_list: pointer on list head, no value
[ 1493.922884] ex_list: {[], 4, 3}
[ 1493.928743] ex_list: added 5
[ 1493.931122] ex_list: added 6
[ 1493.937877] ex_list: added 7
[ 1493.940051] ex_list: added 8
[ 1493.940726] ex_list: pointer on 3
[ 1493.940770] ex_list: {8, 7, 6, 5, 4, [3]}
[ 1493.945944] ex_list: removed 3
[ 1493.948185] ex_list: removed 4
[ 1493.950333] ex_list: removed 5
[ 1493.950991] ex_list: {8, 7, [6]}
[ 1493.956160] ex_list: removed 6
[ 1493.958488] ex_list: removed 7
[ 1493.960811] ex_list: removed 8
[ 1493.961477] ex_list: {[]}
[ 1493.965507] ex_list: unloaded
```
#  Тест 2
проверяет добавление в начало/удаление из конца списка.
  **Ожидаемый результат:**
```
[ 1065.848328] ex_list: loaded
[ 1065.857416] ex_list: added 1
[ 1065.859754] ex_list: added 2
[ 1065.862164] ex_list: added 3
[ 1065.868151] ex_list: added 4
[ 1065.868801] ex_list: {[], 4, 3, 2, 1}
[ 1065.868862] ex_list: pointer on 4
[ 1065.874148] ex_list: removed 4
[ 1065.877313] ex_list: removed 3
[ 1065.878159] ex_list: pointer on 1
[ 1065.878212] ex_list: {2, [1]}
[ 1065.888158] ex_list: added 5
[ 1065.890338] ex_list: added 6
[ 1065.896446] ex_list: added 7
[ 1065.898526] ex_list: added 8
[ 1065.899164] ex_list: pointer on 8
[ 1065.899208] ex_list: {2, 1, [8], 7, 6, 5}
[ 1065.904168] ex_list: removed 8
[ 1065.906647] ex_list: removed 7
[ 1065.909022] ex_list: removed 6
[ 1065.909623] ex_list: {2, 1, [5]}
[ 1065.914854] ex_list: removed 5
[ 1065.917303] ex_list: removed 1
[ 1065.919413] ex_list: removed 2
[ 1065.920173] ex_list: {[]}
[ 1065.925906] ex_list: unloaded
```
#  Тест 3
проверяет добавление в начало/удаление из середины списка.
  **Ожидаемый результат:**
```
[ 1162.988146] ex_list: loaded
[ 1162.994630] ex_list: added 1
[ 1162.997119] ex_list: added 2
[ 1162.999257] ex_list: added 3
[ 1163.001693] ex_list: added 4
[ 1163.002374] ex_list: {[], 4, 3, 2, 1}
[ 1163.007914] ex_list: removed 4
[ 1163.010723] ex_list: removed 3
[ 1163.011504] ex_list: {[], 2, 1}
[ 1163.011569] ex_list: pointer on 2
[ 1163.017594] ex_list: added 5
[ 1163.020560] ex_list: added 6
[ 1163.021313] ex_list: pointer on 6
[ 1163.026741] ex_list: added 7
[ 1163.029160] ex_list: added 8
[ 1163.029832] ex_list: {2, [6], 8, 7, 5, 1}
[ 1163.035381] ex_list: removed 6
[ 1163.037837] ex_list: removed 8
[ 1163.039998] ex_list: removed 7
[ 1163.040649] ex_list: {2, [5], 1}
[ 1163.045554] ex_list: removed 5
[ 1163.047611] ex_list: removed 1
[ 1163.050244] ex_list: removed 2
[ 1163.050846] ex_list: {[]}
[ 1163.055249] ex_list: unloaded

```

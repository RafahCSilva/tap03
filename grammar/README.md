Instruções
==========

Compile o seu programa e chame o arquivo executável de `a.out`.

Alternativamente, crie um script bash de nome `a.out` que funciona como um wrapper para o seu programa. Por exemplo, se seu programa foi feito em Python, e se chama gramatica.py, seu arquivo `a.out` iria conter as seguintes linhas:

```bash
#!/bin/bash
python3 gramatica.py $@
```

Depois de criar o arquivo `a.out`, digite os seguintes comandos num terminal:

```bash
chmod +x a.out
./grade.bash
```



Observações
===========

No nosso caso, como não há especificação de linha de comando, nem precisamos repassar os argumentos, então pode tirar o $@

Na hora de submeter no Moodle, você não precisa submeter esse wrapper, mas apenas o código fonte (no exemplo, gramatica.py).


void alterarCarta(int *tamanho, Cartas lista[])
{
    int ID_numero, escolha, escolha2, atributo_mod;
    char ID_letra;

    do
    {
        printf("Digite o número do ID (1-8): ");
        scanf("%d", &ID_numero);
    } while (ID_numero > 8 || ID_numero < 1);

    setbuf(stdin, NULL);
    do
    {
        printf("Digite uma letra do ID (A - D): ");
        scanf("%c", &ID_letra);
        ID_letra = toupper(ID_letra);
    } while (ID_letra != 'A' && ID_letra != 'B' && ID_letra != 'C' && ID_letra != 'D');

    for (int i = 0; i < *tamanho; i += 1)
    {
        if (lista[i].numero == ID_numero && lista[i].letra == ID_letra)
        {
            exibeCartas(lista, i);
            printf("é essa carta que deseja alterar?");
            printf("1 - Sim");
            printf("2 - Não");
            scanf("%i", &escolha);
            do
            {
                printf("deseja selecionar outra carta?");
                printf("1 - Sim");
                printf("2 - Não");
                scanf("%i", &escolha2);
                if (escolha2 != 1 || escolha2 != 2)
                {
                    printf("Opção invalida, digite novamente: ");
                    scanf("%i", escolha);
                }

                if (escolha2 == 2)
                {
                    break;
                }
                if (escolha2 == 1)
                {
                    alterarCarta(tamanho, lista[]);
                    break;
                }
            } while ((escolha == 2));

            do
            {
                printf("selecione o atribiuto que deseja alterar:");
                printf("1 - legado");
                printf("2 - popularidade");
                printf("3 - estratégia");

                if (atributo_mod == 1)
                {
                    /* code */
                }
                if (atributo_mod == 2)
                {
                    /* code */
                }
                if (atributo_mod == 3)
                {
                    /* code */
                }
                if (atributo_mod != 1 || atributo_mod != 2 || atributo_mod != 3)
                {
                    /* code */
                }
            } while (escolha == 1);

            do
            {
                printf("Opção invalida, digite novamente: ");
                scanf("%i", escolha);
            } while (escolha != 1 || escolha != 2);
        }
    }
};
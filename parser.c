/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:57:38 by mtiago-s          #+#    #+#             */
/*   Updated: 2023/05/10 20:44:18 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "Libft/libft.h"


int	check_sep(char *str, int *slen)
{
	*slen = 0;
	if ((str[0] == '<' && str[1] == '<') || (str[0] == '>' && str[1] == '>'))
		*slen = 2;
	else if (str[0] == '<' || str[0] == '>')
		*slen = 1;
	else if (str[0] == '|')
		*slen = 1;
	return (*slen);
}

int	count_redir_pipe(char c1, char c2, int *rp_len)
{
	*rp_len = 0;
	if ((c1 == '>' && c2 == '>') || (c1 == '<' && c2 == '<'))
		*rp_len = 2;
	else if (c1 == '>' || c1 == '<')
		*rp_len = 1;
	else if (c1 == '|')
		*rp_len = 1;
	return (*rp_len);
}

char	change_quotes(char c, char quote)
{
	if ((c == '\'' || c == '\"') && (quote == 0 || quote == c))
        quote = c * (quote != c);
    return (quote);
}


/* Esta funcao faz o parsing do minishell 
Linha 47: -Se sep encontrado e se encontrado novamente desligado
		if (sep != *str) -> inicializa sep -> sep = 1
		if (sep == *str) -> reinicia sep -> sep = 0;
Linha 49: -Se seo = 0 e ' ' torna em 2 na str
Linha 51: -Se for um token {<, <<, >, >>, |} e sep = 0: 
		  meter um 3 na casa do separador (tem de ser o unico 3 até ver uma 
		  algo diferente de token -> len = 0).
		  -Se nao for um pipe e len>0 --> manter o elemento na str
		  -Se nao for um pipe e slen==2 --> manter o proximo, fazendo dois
		  -No final meter um 2 extra e len passa para 1
Linha 56: Incrementamos o str aqui para poupar linhas
Linha 61: -Caso geral: é ir colando as letras na string res e dizer que 
		  pode começar um novo token -> len = 0*/
void	parse(char *res, char *str, char sep, int slen)
{
	int		len;
    int     i;
    int     j;

    i = 0;
    j = 0;
	len = 0;
	while (&str[i] && str[i])
	{
		sep = change_quotes(str[i], sep);
		if (str[i] == ' ' && !sep)
			str[i] = '2';
		if (!sep && count_redir_pipe(str[i], str[i + 1], &slen)) // sub de pipes por 3 ou no caso de > colocacao de um tres e do proprio cha
		{
			res[j++] = ('3' - (len != 0));
			if (str[i] != '|' || len)
				res[j++] = str[i];
			if (&str[++i] && str[i] != '|' && slen == 2)
				res[j++] = str[i++];
			res[j++] = '2';
			len++;
		}
		else
		{
            if(!(str[i] == '\'' || str[i] == '\"'))
			    res[j++] = str[i++];
            else
                i++;
			len = 0;
		}
	}
}

//Example : wc | cat>out ls
//Output:   wc2322cat3>2out2ls

//Example : cat | "grep a" | >>> something
//Output:  cat2322"grep a"23223>>22>22something

//----------------------------------------------------------------
//--  "Makefile < cat"     -- Makefile < cat: command not found --                        --
//----------------------------------------------------------------
//--  "< Makefile cat"     -- Faz here doc e escreve num        --
//--                       -- ficheiro chamado cat              --
//----------------------------------------------------------------
//--  < Makefile cat       --  Faz cat do Makefile no terminal  --
//----------------------------------------------------------------
//--< Makefile cat > out   -- Faz cat para um ficheiro out      --
//----------------------------------------------------------------
//--"< Makefilecat "< cat""-- < Makefilecat : command not found --
//----------------------------------------------------------------
//--    "ls"< cat""        --  Faz o ls no terminal             --
//----------------------------------------------------------------
//--  "Makefile"< ls"      --  Faz o here doc para lado nenhum  --
//----------------------------------------------------------------
//--     ls >>> cat        --  error near unexpected token `>'  --
//----------------------------------------------------------------
//--     ls <<< ola       --   executa o ls                     --
//----------------------------------------------------------------



/* int main(int ac, char **av)
{
	char *line = readline(">");
	(void)ac;
    char **res3;
	static char res[1000];
	parse(res,line, 0, 0);
    res3 = ft_split(res, '3');
    int i = 0;
    while (res3[i])
    {
        printf("%s\n", res3[i++]);
    }
} */
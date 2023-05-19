/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:06:38 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/19 17:41:29 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

/* int	check_sep(char *str, int *slen)
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
} */

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

int	is_whitespace(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	quote_type(char c)
{
	if (c == '\'')
		return (1);
	else if (c == '\"')
		return (2);
	return (0);
}

int	is_even_quotes(char *str)
{
	int	i;
	int	quotes_open;
	int	flag_type;

	i = 0;
	quotes_open = 0;
	while (str[i])
	{
		if (quote_type(str[i]))
		{
			if (quotes_open == 0)
			{
				flag_type = quote_type(str[i]);
				quotes_open = 1;
			}
			else if (quotes_open == 1)
				if (flag_type == quote_type(str[i]))
					quotes_open = 0;
		}
		i++;
	}
	if (quotes_open == 1)
		return (0);
	return (1);
}


char	*del_quotes(char *str)
{
	int	i;
	int	j;
	int	quotes_open;
	int	flag_type;

	i = 0;
	j = 0;
	quotes_open = 0;
	while (str[i])
	{
		if (quote_type(str[i]))
		{
			if (quotes_open == 0)
			{
				flag_type = quote_type(str[i]);
				quotes_open = 1;
			}
			else if (quotes_open == 1)
				if (flag_type == quote_type(str[i]))
					quotes_open = 0;
		}
		if ((!quote_type(str[i]) && !quotes_open) || (quotes_open && quote_type(str[i]) != flag_type))
		{
			str[j] = str[i];
			j++;
		}
		i++;
	}
	str[j]= '\0';
	return (str);
}

t_arg	*arg_last(t_arg *arg)
{
	if (!arg)
		return (0);
	while (arg->next)
		arg = arg->next;
	return (arg);
}

void	arg_add_back(t_arg **arg, t_arg *new)
{
	t_arg	*last;

	if (!new || !arg)
		return ;
	if (!*arg)
	{
		*arg = new;
		return ;
	}
	last = arg_last(*arg);
	last->next = new;
}

t_arg	*create_arg(t_arg **args)
{
	t_arg	*arg;

	arg = ft_calloc(1, sizeof(t_arg));
	if (!arg)
		return (0);
	arg->name = 0;
	arg->arg_len = 0;
	arg->in_quotes = 0;
	arg->quote_type = '\0';
	arg_add_back(args, arg);
	return (arg);
}

void	parse_aux(t_arg *arg, char *str, int *i)
{
	char	c;
	int		len;
	int		start;
	int 	flag_end;

	start = *i;
	len = ft_strlen(str);
	flag_end=0;
	while (*i < len + 1)
	{
		c = str[*i];
		if (!arg->in_quotes && (c == '\'' || c == '\"'))
		{
			arg->in_quotes = 1;
			arg->quote_type = c;
			(*i)++;
			start++;
			continue ;
		}
		if (arg->in_quotes && c == arg->quote_type)
		{
			arg->in_quotes = 0;
			arg->quote_type = '\0';
			(*i)++;
			flag_end++;
			continue ;
		}
		if ((!arg->in_quotes && is_whitespace(c)) || c == '\0')
		{
			arg->name = ft_substr(str, (unsigned int)start, (size_t)(*i
						- start - flag_end));
			break ;
		}
		(*i)++;
	}
}

t_arg	**parse_arguments(char *string)
{
	int		i;
	int		len;
	t_arg	**args;
	t_arg	*temp;

	if (!is_even_quotes(string))
	{
		printf("error\n");
		return (0);
	}
	len = ft_strlen(string);
	i = 0;
	args = ft_calloc(1,sizeof(t_arg *));
	while (i < len)
	{
		if (!(temp = create_arg(args)))
			return (NULL);
		while (i < len && is_whitespace(string[i]))
			i++;
		parse_aux(temp, string, &i);
	}
	return (args);
}

/* void	parse(char *res, char *str, char sep, int slen)
{
	int	len;
	int	i;
	int	j;

	i = 0;
	j = 0;
	len = 0;
	while (&str[i] && str[i])
	{
		sep = change_quotes(str[i], sep);
		if (is_whitespace(str[i]) && !sep)
			str[i] = '2';
		if (!sep && count_redir_pipe(str[i], str[i + 1], &slen))
		// sub de pipes por 3 ou no caso de > colocacao de um tres e do proprio cha
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
			if (!(str[i] == '\'' || str[i] == '\"'))
				res[j++] = str[i++];
			else
				i++;
			len = 0;
		}
	}
} */

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

int	main(int ac, char **av)
{
	char *line = readline(">");
	(void)ac;
	char **res3;
	char **res31;
	static char res[1000];
	t_arg **args;

	line = del_quotes(line);

	printf("%s", line);

	/* while(args && (*args))
	{

		printf("%s\n", (*args)->name);
		(*args) = (*args)->next;
	} */


	/*parse(res, line, 0, 0);
	res3 = ft_split(res, '3');
	int i = 0;
	while (res3[i])
	{
		res31 = ft_split(res3[i], '2');
		printf("%s\n", res3[i++]);
	}
	i = 0;
	while (res31[i])
    {
        printf("%s\n", res31[i++]);
    } */
}
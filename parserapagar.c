/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserapagar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:06:38 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/19 23:38:47 by pedperei         ###   ########.fr       */
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

int	is_pipes_red(char c)
{
	if (c == '|' || c == '<' || c == '>')
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
		if ((!quote_type(str[i]) && !quotes_open) || (quotes_open
				&& quote_type(str[i]) != flag_type))
		{
			str[j] = str[i];
			j++;
		}
		i++;
	}
	str[j] = '\0';
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
	arg->quote_type = 0;
	arg_add_back(args, arg);
	return (arg);
}

int	edit_parse_struct(t_arg *arg, int *i, char c, int quotes)
{
	arg->in_quotes = quotes;
	arg->quote_type = c;
	(*i)++;
	return (1);
}

int	parse_pipe_red(t_arg **args, int *i, char *str, char *sep)
{
	char	c;
	t_arg	*temp;

	c = str[*i];
	temp = create_arg(args);
	temp->name = ft_strdup(sep);
	(*i)++;
	return (1);
}

int	parsing_tree(t_arg **args, int *i, char *str)
{
	if (str[*i] == '|')
		return (parse_pipe_red(args, i, str, "|"));
	if (ft_strncmp(&str[*i], ">>", 2) == 0)
	{
		(*i)++;
		return (parse_pipe_red(args, i, str, ">>"));
	}
	if (ft_strncmp(&str[*i], "<<", 2) == 0)
	{
		(*i)++;
		return (parse_pipe_red(args, i, str, "<<"));
	}
	if (str[*i] == '>')
		return (parse_pipe_red(args, i, str, ">"));
	if (str[*i] == '<')
		return (parse_pipe_red(args, i, str, "<"));
	return (0);
}

/* ft_strncmp(&str[*i], ">>", 2) == 0
if (!(temp_arg = create_arg(args)))
				return ;
			temp_arg->name = ft_strdup(">>");
			(*i)++;
			(*i)++;
			break ; */

void	eliminate_extra_arg(t_arg **arg)
{
	t_arg	*temp;
	t_arg	*head;
	t_arg	*end;

	temp = (*arg);
	head = (*arg);
	while (temp->next != NULL)
	{
		if (temp->next->next == NULL)
			end = temp;
		temp = temp->next;
	}
	free(temp);
	end->next = NULL;
}

void	parse_aux(t_arg **args, t_arg *arg, char *str, int *i)
{
	char	c;
	char	*temp;
	int		change;

	arg->name = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	while (str[*i])
	{
		change = 0;
		c = str[*i];
		if (!arg->in_quotes && is_whitespace(c))
			break ;
		if (!arg->in_quotes && quote_type(c) && !change)
			change = edit_parse_struct(arg, i, c, 1);
		if (arg->in_quotes && arg->quote_type == c && !change)
			change = edit_parse_struct(arg, i, 0, 0);
		if (!arg->in_quotes && is_pipes_red(c))
		{
			if (ft_strcmp(arg->name, "") == 0)
				eliminate_extra_arg(args);
			change = parsing_tree(args, i, str);
			break ;
		}
		if (change)
			continue ;
		arg->name[arg->arg_len++] = c;
		(*i)++;
	}
	temp = ft_strdup(arg->name);
	free(arg->name);
	arg->name = temp;
}

t_arg	**parse_arguments(char *string)
{
	int		*i;
	int		len;
	t_arg	**args;
	t_arg	*temp;

	if (!is_even_quotes(string))
	{
		printf("error\n");
		return (0);
	}
	len = ft_strlen(string);
	i = ft_calloc(1, sizeof(int));
	args = ft_calloc(1, sizeof(t_arg *));
	while (*i < len)
	{
		while (*i < len && is_whitespace(string[*i]))
			(*i)++;
		if (!(temp = create_arg(args)))
			return (NULL);
		parse_aux(args, temp, string, i);
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

/* int	main(int ac, char **av)
{
	char *line = readline(">");
	(void)ac;
	char **res3;
	char **res31;
	static char res[1000];
	t_arg **args;

	args = parse_arguments(line);

	//printf("%s", line);

	while (args && (*args))
	{
		printf("%s\n", (*args)->name);
		(*args) = (*args)->next;
	}
} */
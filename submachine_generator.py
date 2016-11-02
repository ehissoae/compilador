# -*- encoding:utf-8 -*-

import re, json

input_file_name = "output_barauna.txt"
output_file_name = "submachines.h"

initial_submachine = 'comandos'

input_file = open(input_file_name, "r")

submachines = []
tokens = set()

# gera uma string no formato de inicialização de matrizes em C
def stringfy_3d_matrix(matrix):
	str_list = []

	str_list.append('\n{')
	for i, m1 in enumerate(matrix, 1):
		str_list.append('\n\t{')
		for j, m2 in enumerate(m1, 1):
			str_list.append('\n\t\t{')
			for k, elem in enumerate(m2, 1):
				str_list.append(str(elem))
				if k != len(m2):
					str_list.append(', ')

			str_list.append('}')
			if j != len(m1):
				str_list.append(',')

		str_list.append('\n\t}')
		if i != len(matrix):
			str_list.append(',')

	str_list.append('\n};')
	return ''.join(str_list)


# lê as transições no formato do arquivo output_barauna.txt
def get_from_to_consume(string):
	match = re.match(r'\((\d+), ([^\)]+)\) -> ((\d+))', string)

	if not match:
		match = re.match(r'\((\d+), ("\)")\) -> ((\d+))', string)

		if not match:
			import pdb; pdb.set_trace()

	return int(match.groups()[0]), int(match.groups()[2]), match.groups()[1]






# lemos o arquivo linha por linha,
# guardando em um dicionario mais facil de se manipular
for line in input_file:
	line = line.strip()

	if line.startswith("Automata - "):
		submachine_dict = {
			'name': line.replace('Automata - ', ''),
			'final_states': None,
			'states': set([0]),
			'transitions': [],
			'transitions_dict': {},
		}

		submachines.append(submachine_dict)

	elif line.startswith("final"):
		submachine_dict['final_states'] = set([int(x) for x in line.replace('final: ', '').split(', ')])

	elif line.startswith("("):
		from_state, to_state, token_or_machine = get_from_to_consume(line)

		transition = {
			'from': from_state,
			'to': to_state,

			# 'submachine': None,
			# 'token': None,
			'consume': None
		}

		if token_or_machine.startswith('"'):
			# transition['token'] = token_or_machine
			transition['consume'] = ('token', token_or_machine)
			tokens.add(token_or_machine)
		else:
			# transition['submachine'] = token_or_machine
			transition['consume'] = ('submachine', token_or_machine)

		submachine_dict['transitions'].append(transition)
		submachine_dict['states'].add(from_state)
		submachine_dict['states'].add(to_state)


output_file = open(output_file_name, "w")

# começando a escrever o .h
# import necessario par ao strcmp
output_file.write('#include <string.h>\n\n')

# comentario com a possição de cada submaquina na matriz em C
output_file.write('// - submachines\n')
submachine_name_to_number = {}
for number, x in enumerate(submachines):
	if x == initial_submachine:
		output_file.write('\t// %d = %s - INICIAL!\n' % (number, x['name']))
	else:
		output_file.write('\t// %d = %s\n' % (number, x['name']))
	submachine_name_to_number[x['name']] = number

output_file.write('\nconst int INITIAL_SUBMACHINE = %d;\n' % submachine_name_to_number[initial_submachine])


# devolve o conjunto de tokens que a submaquina pode gerar na "primeira posição"
def first(submachine):
	transitions = submachines[submachine_name_to_number[submachine]]['transitions']
	tokens = set()

	# print 'first: ' + submachine
	for transition in transitions:
		if transition['from'] == 0:
			if transition['consume'][0] == 'token':
				tokens.add(transition['consume'][1])
			else:
				# print 'first(%s): ' % transition['consume'][1]
				tokens = tokens.union(first(transition['consume'][1]))

	# print tokens
	return tokens

# coloca as transições da submaquina em um formato mais facil para depois escrever em C
def make_transitions_dict(submachine):
	transitions = submachines[submachine_name_to_number[submachine]]['transitions']
	transitions_dict = {}

	for transition in transitions:
		from_state = transition['from']
		to_state = transition['to']
		consume = transition['consume']

		if from_state not in transitions_dict:
			transitions_dict[from_state] = {}

		tokens = None
		submachine_call = -1
		if consume[0] == 'token':
			tokens = set([consume[1]])
		else:
			tokens = first(consume[1])
			submachine_call = consume[1]

		for token in tokens:
			exists = transitions_dict[from_state].get(token)
			if exists:
				print "ERROR! Ambiguidade com lookahead de 1!!"
				import pdb; pdb.set_trace()

			transitions_dict[from_state][token] = (to_state, submachine_call)

	return transitions_dict


# escrevendo no arquivo a função symbol_table
# ela recebe o texto de um token e devolve o indice corresponde a ser usado na matriz
output_file.write('\n')
output_file.write('// - tokens\n')
token_name_to_number = {}
symbol_table_string = """\nint symbol_table(char* str){
"""
tokens = list(tokens)
for number, token in enumerate(tokens):
	output_file.write('\t// %d = %s\n' % (number, token))
	token_name_to_number[token] = number
	if token != '"identificador"':
		symbol_table_string += """\tif(strcmp(str, %s)==0){
\t\treturn %d;
\t}
""" % (token, number)
token_name_to_number[token] = number

symbol_table_string += "\treturn %d;//identificador\n" % token_name_to_number['"identificador"']
symbol_table_string += '}\n'

output_file.write(symbol_table_string)


# para cada posição na matriz criamos a info {estado_destino, submaquina_chamada}
max_num_states = max([len(x['states']) for x in submachines])
transition_table = []
for i, submachine_dict in enumerate(submachines):
	submachine_name = submachine_dict['name']
	num_states = len(submachine_dict['states'])

	transitions_dict = make_transitions_dict(submachine_name)
	transition_array_2 = []

	for from_state in xrange(0, max_num_states):
		# transitions = transitions_dict[from_state]
		transition_array_1 = []

		for token in tokens:
			if from_state in transitions_dict and token in transitions_dict[from_state]:
				to_state, submachine_call = transitions_dict[from_state][token]
				if submachine_call != -1:
					submachine_call = submachine_name_to_number[submachine_call]
				transition_array_1.append('{%2d,%2d}' % (to_state, submachine_call))
			else:
				# print submachine_dict['final_states']
				if from_state in submachine_dict['final_states']:
					transition_array_1.append('{-2,-2}')
				else:
					transition_array_1.append('{-1,-1}')

		transition_array_2.append(transition_array_1)

	transition_table.append(transition_array_2)

output_file.write('// {-1, -1} significa um erro\n')
output_file.write('// {X, -1} significa que eh uma transicao que consome o token\n')
output_file.write('// {X, Y} significa que eh uma chamada da submaquina Y, nao consome o token\n')
output_file.write('// {-2, -2} significa que eh um retorno de submaquina\n')
output_file.write('\nint APE_TRANSITION_TABLE[%d][%d][%d][2] =' % (len(submachines), max_num_states, len(tokens)))
output_file.write(stringfy_3d_matrix(transition_table))




output_file.close()
import pdb; pdb.set_trace()
import tkinter as tk
from tkinter import filedialog

file_select_addr = ''
file_select = []        #list containing lines of the selected file
file_assembled = []     #list containing lines of the assembled file

inst =      [ 'ADD',    'SUB',      'MUL',      'AND',      'OR',       'NOT',      'XOR',      'SHL',      'SHR',      'WM',       'RM',       'RIO',      'WIO',      'WB',       'WIB',      'WACC',     'RACC',     'SWAP',     'BR',       'BRE',      'BRNE',     'BRGT',     'BRLT',     'EOP' ]
inst_code = [ '11110',  '11101',    '11011',    '11010',    '11001',    '11000',    '10111',    '10110',    '10101',    '00001',    '00010',    '00100',    '00101',    '00110',    '00111',    '01001',    '01011',    '01110',    '00011',    '10100',    '10011',    '10010',    '10001',    '11111' ]
no_ops =    [ 'ADD', 'SUB', 'MUL', 'AND', 'OR', 'NOT', 'XOR', 'SHL', 'SHR', 'WACC', 'RACC', 'SWAP', 'EOP' ]

errors = []

addr_ptr = 0
pointer_name = []
pointer_addr = []

def open_file_dialog():
    file_path = filedialog.askopenfilename()
    if file_path:
        global file_select_addr
        file_select_addr = file_path

        file_entry.delete(0, tk.END)  # Clear the entry field
        file_entry.insert(0, file_path)  # Insert the selected file path into the entry field

def save_file():
    file_path = filedialog.asksaveasfilename(defaultextension=".txt")
    global file_select
    file_select.clear()
    global file_assembled
    file_assembled.clear()
    with open (file_select_addr, 'rt') as selected_file:
        file_select = selected_file.readlines()
        selected_file.close()
    assemble()
    if file_path:
        with open(file_path, 'w') as output_file:
            if errors == []:
                for line in file_assembled:
                    output_file.write(line+'\n')
                output_label.config(text='ASSEMBLED SUCCESFULLY')
            else:
                output_file.write('/* ERROR(s) EXIST\n')
                for line in errors:
                    output_file.write(line+'\n')
                output_label.config(text='Errors exist! Check file for log')
            output_file.close()

def assemble():
    global addr_ptr
    for i, line in enumerate(file_select):
        line = line.lstrip()
        line = line.upper()
        if line == '':          continue                        #if line is empty, continue
        if ';' in line:         line = line[:line.index(';')]   #if line contains comment, remove comment

        parts = line.split()                                    #split lines into array 'parts'
        instruction = ''                                        #use this to store instruction code

        while parts != []:
            if 'ORG' in parts[0]:                               #if ORG, change the address to write on
                temp = convert_to_binary(parts[1], 11)
                if temp == 'error':     
                    errors.append(f'Error at line {i} near ORG')
                    parts = parts[2:]
                    continue
                else:
                    global addr_ptr
                    addr_ptr =  int( temp, 2)
                    parts = parts[2:]
                continue

            if ':' in parts[0]:                                 #if label, store the label in pointer_name
                pointer_name.append(parts[0][:parts[0].index(':')])
                pointer_addr.append(addr_ptr)
                parts = parts[1:]
                continue

            if parts[0] in inst:                                #if instruction is recognized
                instruction += inst_code[ inst.index(parts[0])]     #add the instruction code
                if parts[0] in no_ops:                          #if instruction does not use operand
                    instruction += '00000000000'
                    # toWrite=f'ADDR = {hex(addr_ptr)}; BUS = {hex(int(instruction[:8], 2))}; MainMemory(); //{file_select[i].rstrip()}'
                    toWrite='ADDR = 0x{:03X}; BUS = 0x{:02X}; MainMemory();\t//{}'.format( addr_ptr, int(instruction[:8], 2), file_select[i].rstrip())
                    addr_ptr+=1
                    file_assembled.append(toWrite)

                    # toWrite=f'ADDR = {hex(addr_ptr)}; BUS = {hex(int(instruction[8:], 2))}; MainMemory();'
                    toWrite='ADDR = 0x{:03X}; BUS = 0x{:02X}; MainMemory();'.format(addr_ptr, int(instruction[8:], 2))
                    addr_ptr+=1
                    file_assembled.append(toWrite)
                    parts = parts[1:]
                    continue

                if parts[1] in pointer_name:    
                    temp = format( pointer_addr[ pointer_name.index(parts[1]) ], '#013b')[2:]
                else:                           temp = convert_to_binary( parts[1], 11 )
                if temp == 'error':             
                    errors.append(f'Error at line {i} near {parts[0]}')
                    parts = parts[2:]
                    continue
                else:   
                    instruction += temp #add the operand
                
                # toWrite=f'ADDR = {hex(addr_ptr)}; BUS = {hex(int(instruction[:8], 2))}; MainMemory();   //{file_select[i][:-1]}'
                toWrite='ADDR = 0x{:03X}; BUS = 0x{:02X}; MainMemory();\t//{}'.format(  addr_ptr, int(instruction[:8], 2), file_select[i][:-1] )
                addr_ptr+=1
                file_assembled.append(toWrite)

                # toWrite=f'ADDR = {hex(addr_ptr)}; BUS = {hex(int(instruction[8:], 2))}; MainMemory();'
                toWrite='ADDR = 0x{:03X}; BUS = 0x{:02X}; MainMemory();'.format(    addr_ptr, int(instruction[8:], 2) )
                addr_ptr+=1
                file_assembled.append(toWrite)
                parts = parts [2:]
                continue
            
            errors.append(f'Error at line {i}. Unknown Command')
            parts = parts[2:]
            continue

def convert_to_binary(number_str, num_digits):
    try:
        if number_str.lower().endswith('h'):        binstr = bin(int(number_str[:-1], 16))[2:]        # Hexadecimal format
        elif number_str.lower().startswith('0x'):   binstr = bin(int(number_str, 16))[2:]             # Hexadecimal format (with 0x prefix)
        elif number_str.lower().endswith('b'):      binstr = number_str[:-1]                          # Binary format
        elif number_str.lower().startswith('0b'):   binstr = number_str[2:]                           # Binary format (with 0b prefix)
        elif number_str.lower().endswith('d'):      binstr = bin(int(number_str[:-1]))[2:]            # Decimal format
        elif number_str.lower().startswith('0d'):   binstr = bin(int(number_str[2:]))[2:]             # Decimal format (with 0d prefix)
        else:                                       return 'error'
        if len(binstr) < num_digits:                binstr = '0' * (num_digits - len(binstr)) + binstr
        elif len(binstr) > num_digits:              return 'error'
        return binstr
    except: return 'error'
        

#--------------- START OF USER INTERFACE STUFF ------------------
# Create the main application window
root = tk.Tk()
root.title("File Input")

# Create a title label
title_label = tk.Label(root, text="BEEG ASSEMBLY ASSEMBLER 3000")
title_label.pack(pady=10)

output_label = tk.Label(root, text="Pick a file then press 'Conduct Assembly'")
output_label.pack(pady=10)

# First line
frame1 = tk.Frame(root)
frame1.pack()

# Create textbox and browse button
file_entry = tk.Entry(frame1, width=50)
file_entry.pack(side=tk.LEFT, padx=5)

browse_button = tk.Button(frame1, text="Browse", command=open_file_dialog)
browse_button.pack(pady=5, side=tk.RIGHT)

# Create a button to process the file
save_file_button = tk.Button(root, text="Conduct Assembly", command=save_file)
save_file_button.pack(pady=10)

# Run the Tkinter event loop
root.mainloop()
#--------------- END OF USER INTERFACE STUFF ------------------
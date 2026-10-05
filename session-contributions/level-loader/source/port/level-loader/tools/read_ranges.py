import pathlib,sys
p=pathlib.Path(sys.argv[1]);lines=p.read_text().splitlines()
for span in sys.argv[2:]:
    a,b=map(int,span.split(':'))
    print('\n'.join(f'{i+1}: {lines[i]}' for i in range(a-1,min(b,len(lines)))))

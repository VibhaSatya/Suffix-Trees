f=open("AesopTales.txt","r");
l =[]

for line in f:
	s=str(line)
        line=s.replace("\r\n","#")
	l.append(line)
        
#print(l)

#end of document
for i in range(0,len(l)-1):
	if((l[i]=="#" or l[i]==" #") and (l[i+1]=="#" or l[i+1]==" #")):
		l[i+1]="$"

for i in l:
	if(i=="#"):
		l.remove(i)

print(l)
title=[]

for i in range(0,len(l)-1):
	if(l[i][-1]=="#" and (l[i+1]==" #" or l[i+1]=="#") and l[i][0].isupper()):
		s=str(l[i])
		l[i]=s.replace(" #",".")
		
	if("#" in l[i]):
		s=str(l[i])
		l[i]=s.replace("#"," ")

for i in l:
	if(i==" "):
		l.remove(i)

title.append(l[0])	
for i in range(0,len(l)-1):
	if(l[i]=="$"):
		title.append(l[i+1]);	
	
s=str(l[-1])
l[-1]=s.replace("#","$")

for i in range(0,len(l)):
	if("   " in l[i]):
		s=str(l[i])
		l[i]=s.replace("   "," ")
	if("  " in l[i]):
		s=str(l[i])
		l[i]=s.replace("  "," ")
	if(". ." in l[i]):
		s=str(l[i])
		l[i]=s.replace(". .",".")

		
		
			
#print(l)
#print(title)
f.close()

f=open("cleanAesop.txt","w");
for i in l:
	f.write(i);

f.close()
f=open("titles.txt","w");
count=1
for i in title:
	f.write(str(count))
	f.write(" "+i);
	f.write("\n");
	count=count+1
f.close()

count=1;
doc="doc"+str(count)
f=open(doc+".txt","w")
for i in l:
	f.write(i)
	if("$" in i):
		count=count+1
		f.close()
		doc="doc"+str(count)
		f=open(doc+".txt","w")
		
f.close()	
	

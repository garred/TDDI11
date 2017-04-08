# 1.	Assume	a	circle	around	the	current	mouse	position.	Will	any	straight-line	movement	that	moves	the	mouse	from	its	current	position	at	the	center	of	the	circle	to	arbitrary	points	on	the	circle	have	the	same	effect	on	the	“motionMetric”?	Why?

No, because it will depend on the history of previous movements, since `motionMetric` is a low-pass filter.


# 2.	Name	an	alternative	for	the	Euclidean	metric	that	we	used	(i.e.,	$\sqrt{h^2 + v^2}$ ).	Briefly	explain	how	it	works.

Another metric is Manhattan, which is the sum of the absolute values of the differences in each dimension.


# 3.	What	are	advantages	and	disadvantages	of	Euclidean	metric?

- Euclidean distance is equal in any direction. This property is interesting for the purpouse of the system we want to make. Manhattan metric is not equal in all directions (https://en.wikipedia.org/wiki/Norm_(mathematics)#Properties).
- In high-dimensional settings it could be more interesting to use Manhattan metric instead of Euclidean, because the lower the exponent of the norm, the more consideration is given to all the dimension (https://www.quora.com/What-is-the-difference-between-Manhattan-and-Euclidean-distance-measures).


# 4.	Modify	the	code,	compile,	and	run	it	for	alpha	equal	to	0.01,	0.5,	and	0.99.	Move	the	mouse	around	with	minor	and	major	movements	and	analyze	the	system	behavior.	You	may	find	it	helpful	to	uncomment	some	of	the	lines	in	“main.c”	to	print	out	more	details.	Name	a	difference	that	each	one	of	these	alpha	values	causes	in	the	system	behavior.	Choose	an	alpha	value	that	you	think	is	useful.

- With alpha = 0.01, it detects any movement of the mouse.
- With alpha = 0.5, it only detects wide movements around the screen.
- With alpha = 0.99, detects also wide movements around the screen.

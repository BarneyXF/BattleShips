Build information.

1) Open BattleShips solution.

2) Setting configuration type to create static lib:
	a) Right-click on AI project and select 'Properties'.
	b) In "General" tab set "Configuration type" to "Static library (.lib)".
	c) Press "OK".
	d) Build AI project.

3) Adding AI.lib to ConsoleApplication1 project:
	a) Right-click on ConsoleApplication1 project and select 'Properties'.
	b) If you did item "2).d)" of this guid using:
		I) x86 configuration - your lib should be in "BattleShips\Debug".
		II) x64 configuration - your lib should be in "BattleShips\x64\Debug".
	   In "Linker\General" tab set "Additional libraries directories" to path in "3).b).I)" or "3).b).II)".

4) Setting configuration type to create static lib:
	a) Right-click on Console project and select 'Properties'.
	b) In "General" tab set "Configuration type" to "Static library (.lib)".
	c) Press "OK".
	d) Build Console project.

5) Adding Console.lib to ConsoleApplication1 project:
	a) Right-click on ConsoleApplication1 project and select 'Properties'.
	b) If you did item "2).d)" of this guid using:
		I) x86 configuration - your lib should be in "BattleShips\Debug".
		II) x64 configuration - your lib should be in "BattleShips\x64\Debug".
	   In "Linker\General" tab set "Additional libraries directories" to path in "3).b).I)" or "3).b).II)".

6) Adding AI.lib to ConsoleApplication1 project's dependencies:
	a) Go to "Linker\Input" tab and set "Additional dependencies" to "AI.lib;"
	b) Press "OK".

7) Adding Console.lib to BattleShips project's dependencies:
	a) Go to "Linker\Input" tab and set "Additional dependencies" to "Console.lib;"
	b) Press "OK".

Now solution is ready and you can run it. 

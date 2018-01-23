
# The InnoVisioNate Graphics System
----

This one is my lifelong passion - or, put another way - something I've been stuck with for what seems like forever !

I don't know why - but for some reason, Sibelius and Scientific Computer Graphics grabbed my ass sometime in 1980 or so. And, to this date, almost 40 years later, they both still have full control over me.

I should have known when, having dropped a stack of FORTRAN punch cards - yes - punch cards (and YES, FORTRAN), I studiously put them ALL back in order absolutely adament about saving that work - that I was forever in the 
unrelenting grip of software development and that I would never ever be free from it ! I wonder now, that if at THAT point time I could see the future 40 years hence, should I have hitailed my ass out of there right then and there ?

I'll never know the answer to that - but I do know that for whatever angst, trials, and tribulations I've endured during these 40 years - if there will be some good that comes out of it - it may as well be a
contribution back to that realm that somehow roped me into this bullshit, irratitating, and frustrating career that I love so much...

And, so, here it is - number graphics - on Windows - in COM - and when I say COM - I mean COM.

## What is this software ?
----

This software is <strong>both</strong> a full featured scientific numeric plotting application/system <strong>AND</strong> a development tool - with which you can add every bit of these features to <strong>YOUR</strong> application.
It is intended to be embedded into other systems by developers who want full featured number graphics at their fingertips. Yet it needs very little 
"container" support. In fact, the "application" that installs with this system is itself a very simple container of perhaps 200 or so lines of code. That application was primarily written as an 
example to show you HOW to embedd the control, but it is powerful enough at the same time to <strong>BE</strong> the hosting application.

It is incredibly easy to pop this tool into just about any system imaginable. 
You can even instantiate this directly in MS Excel with no programming at all ! It has been a lifelong goal of mine to rid the earth of those "Excel" charts you see everywhere - this does that nicely.

As I mentioned, this is COM. To further clarify, this is RAW COM.

All I can say is welcome to this incredibly powerful world ! Let me help you explore this world that MS doesn't have the courtesy to sufficiently document. Seems like all they wanted to do is provide the NEXT generation - 
having left THIS one in such poorly documented state that you'd HAVE to  buy the next !

But you will find - as I did - that it is ultimately SO simple.

So, my friends, join me in this - embrace the fact that software development of integrated systems is complex, yet, given a willingness to understand it, it becomes simple, elegant, robust, and extensible.


### Run the installer
----
You will find the NSIS built installer in the "Installer Support" directory. This installer will put a "Graphic" icon on your desktop. 
You can get to know how the system works quickly by right clicking on the empty graphic and choosing "Functions", then click "Add" to define a new function (default 3-D functions are provided).
You can also integrate directly with excel. To do this, right click, choose properties, then DataSets. When you add a dataset you can import existing data from Excel workbooks.
You can also export datasets to Excel, the data generated,from a function, for example.

The application installed is really just a sample showing how you can embedd this tool in any windows application. The source for that application is in the "GraphicHost" sub-directory. 

Having run the installer - all the binaries in the system will have been registered, so you can begin build and test cycles for each component one at a time.

I recommend running Visual Studio as administrator so that, as each artifact is built, it can be registered in a post-build step, thus, negating the need for an install cycle.

### Get other components
----
Please also clone the Common repository on this site and put it off the root of a drive - by the way, the same as THIS repository.

For example - I have configured the build systems to expect a "\Common" directory to be visible on the same drive that this repository is cloned to. It probably doesn't matter where you put this repository,
but I think it probably best to to put it in "\Graphic" if you have issues.

You MAY want to also clone the Properties repository, again, to the "\Properties" directory on some drive. The properties component is used very heavily throughout this and all of my work. It is a powerful tool 
to provide persistence and UI capabilities to these systems, but it is also relatively static - it has hardly changed in more than 20 years since it's initial development and you can usually assume it doesn't need to be built.
When you ran the installer, the Properties.ocx component was installed and registered.

### Circular dependencies
----
Well - there's no getting around it - there <strong>ARE</strong> circular dependencies here.

The fact is, this is a system of rich objects - sometimes defined in terms of each other - and that is how I want them to be. For example, a dataset has a related plot, a plot has a related dataset, a function is a dataset, and so on. 
I cannot allow a technical issue such as the occassional build difficulty dictate the nature of my objects - if somebody knows a better way, and it doesn't drive my thinking or architecture in some artificial way - I'm all ears, but
for now I live with it.

Note that the systems interrelate through the generated <fileName>_i.h and <fileName>_i.c files from the MIDL compiler. I DO check these into the repositories for the very reason that they need to be there for the first build. Note, 
however, that because they are generated, a build "clean" or "rebuild all" will delete them. As I say, if anyone knows a better way - that does NOT drive the object model, I'd love to hear it.

<strong>By the way</strong> ALL artifacts build into subdirectories of "\Common\Artifacts" - this includes generated files needed across projects.

Anyway - there ARE issues with MIDL puking on just about everything with absolutely no useful information. I have not always been able to get interfaces formally into '.odl files - for example, sometimes I have to resort
to passing void pointers in leiue of the interface because I just can't figure out what it is that MIDL is complaining about - it's diagnostics are total bullshit and it's way past time that we should be given the respect, by MS 
and others, for that matter, to be provided better tools, or at least tools that can be the best they can be.

### Building the system
----
Because of the aforementioned cicular dependencies - please don't do any "build clean", or "rebuild all" quite yet. Someday I'll figure out a better way - but for now, I generally just right click and build each project as necessary.

Also note that all of my projects use a common VS properties sheet that is in the \Common repository (Compiler Options.props) I try very carefully never to apply specific build properties on a file-by-file basis, but sometimes
they slip through.

A quick synopsis of the components in the system is (ALL of these are COM objects):

1. Graphic - the core "central" component - hosts the graphic drawing window and UI components for interacting with Functions, DataSets, Plots and properties for the whole system. This is the main ActiveX/COM component control of the system.
2. Axis - The x, y, and z axiis for the plots.
3. DataSet - The data. That which will be plotted. This object also has very extensive import/export MS Excel capabilities.
4. Documentation - just a place holder for now, some stuff I had started in the early 2000s time frame, but documentation IS lacking here
5. Evaluator - Evaluates mathematical expressions. Given a string representation of any algebraic equation - will provide the results of it. It will detect independent variables that the user would need to provide range(s) for.
6. Function - tightly integrated with the Evaluator to provide the ability to define arbitrarily complex algebraic functions whose data is then tightly integrated with Graphic to achieve plotting (as well as the UI host for the function definitions)
7. GSystem - a few interfaces and other definitions that are useful system wide
8. GraphicHost - The sample C++ embedding application. This very simple application is the "application" installed by the installer. By containing (hosting) the Graphic Control - it is in itself a very powerful graphics system. You would almost think
that this executable WAS the system - but it's not - any windows application can use the Graphic control and appear just as full featured.
9. OpenGLImplementation - All calls to the OpenGL library are made here - essentially nothing in the entire system <i>except</i> this component knows or cares even a little bit about OpenGL. Instead, other components use the interfaces 
to this object to produce graphics. Essentially - one could use any underlying graphics API by re-implementing this component over that API. This particular project shows one way how to access OpenGL in a multi-threaded system correctly.
10. Plot - The graphic itself - the drawing, or "representation" of the data. Note there are only a few plot "types" defined at this point, natural (line), wireframe, surface (again you can see these almost instantly by running the 
sample application and creating functions). However - What is important is that the plot types themselves <strong>are also</strong> COM objects ! In other words - Anybody can create any new plottype they can dream up (in any language For
that matter) and it will seamlessly integrate into the entire system - as if it were natively built in - this is but one very good example of the power of raw COM.
11. PlotTypes - Here are the currently implemented plot types. All of the logic for representing the data would be in COM objects - like this one. This particular project came about when I realized the power of taking this logic out of the main System
and creating a COM-centric strategy that would allow anyone to publish a plot type. If you want to create a new plot type - use this project as a pattern - and create your own (new) project for your type(s). Check out the code in dllMain.cpp
to see how plot types are "registered" so that the main system control (Graphic) can pick them up and implement them. They are actually put into service by the Plot project - but that is just a detail.
12. Text - I have isolated Text Objects here - Text is working pretty well - however, I would like to see better text layout that follows 3-D views - in other words, full control over the 3-D location of the text as well as perspective 
to the view. For example, if the text is on some X-Y plane, then I want it to appear as such in a 3-D View. I believe this is possible with GDI text but I haven't tried it yet. Instead I have attempted to "plot" text by unraveling the
actual glyphs in true-type fonts - but at this point the polygon-fill implementation isn't correct so it looks "hollow".
13. V - This stands for Variable. It is an object that the Function and Evaluator objects will detect and create while parsing some expression. It also contains the UI for range specification when using Functions in the Graphic.
14. ViewSet - This project pops up a dialog that allows you to set the 3-D "viewing parameters" of the scene. You can rotate about 3 different axis using slider controls. Select "Dimension -> Set 3-D View" from the right-click context menu to 
activate the dialog.








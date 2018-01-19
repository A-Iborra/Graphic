
# The InnoVisioNate Graphics System
----

This one is my lifelong passion - or, put another way - something I've been stuck with for what seems like forever !

I don't know why - but for some reason, Sibelius and Scientific Computer Graphics grabbed my ass sometime in 1980 or so. And, to this date, almost 40 years later, they both still have full control over me.

I should have known when, having dropped a stack of FORTRAN punch cards - yes - punch cards (and YES, FORTRAN), I studiously put them ALL back in order absolutely adament about saving that work - that I was forever in the 
unrelenting grip of software development and that I would never ever be free from it ! I wonder now, that if at THAT point time I could see the future 40 years hence, would I get the fuck out of it right then and there !? 

I'll never know the answer to that - but I do know that for whatever angst, trials, and tribulations I've endured during these 40 years - if there will be some good that comes out of it - it may as well be a
contribution back to that realm that somehow roped me into this bullshit, irratitating, and frustrating career that I love so much...

And, so, here it is - number graphics - on Windows - in COM - and when I say COM - I mean COM.

## What is this software ?
----

At it's core - this is really a <strong>development</strong> tool. It is intended to be embedded into other systems by developers who want full featured number graphics at their fingertips. Yet it needs very little 
"container" support. In fact, the "application" that installs with this system is a very simple container of perhaps 100 or so lines of code. It is incredibly easy to pop this tool into just about any system imaginable. 
In fact, one can instantiate this directly in MS Excel with no programming at all ! It has been a lifelong goal of mine to rid the earth of those "Excel" charts you see everywhere - this does that nicely.

As I mentioned, this is COM. To further clarify, this is RAW COM.

All I can say is welcome to this incredibly powerful world ! Let me help you explore this world that MS doesn't have the courtesy to sufficiently document. Seems like all they wanted to do is provide the NEXT generation - 
having left THIS one in such poorly documented state that you'd HAVE to  buy the next !

But you will find - as I did - that it is ultimately SOoooo... simple.

So, my friends, join me in this - embrace the fact that software development of integrated systems is complex, yet, given a willingness to understand it, it becomes simple, elegant, robust, and extensible.

If what you want is cookie cutter, your hand held, it done for you, or some high level lowest common denominator one size fits all thing - well, then, please <strong>Move along - these are not the Droid's you're looking for</strong>

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








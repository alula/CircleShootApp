The PopCap Games Framework provides interfaces to some third-party 
libraries, but licenses to those libraries are not automatically 
granted to you through use of the Framework.  Listed below is 
information for libraries which require explicit licensing if you 
release a product which uses them.

BASS Audio Library
The BASS Audio Library is the default library used by the Framework 
to decompress sounds and to play music.  Licenses to the BASS Audio 
Library can be purchased at http://www.un4seen.com/bass.html#license.
You can avoid using the BASS Audio Library by setting 
SexyAppBase::mWantFMod before application initialization to use the 
FMOD Music Library or by setting SexyAppBase::mNoSoundNeeded if you 
do not need to load compressed audio or play tracked music.
				
FMOD Audio Library
The FMOD library is a popular alternative to the BASS Audio Library.
Licenses to FMOD can be purchased at http://www.fmod.org/. Although 
the BASS Audio Library is the default sound library in the Framework, 
you can use FMOD instead by setting SexyAppBase::mWantFMod before 
application initialization.

MP3 Compressed Audio
The Framework supports both MP3 and OGG decompression through either 
BASS or FMOD.  The OGG format can be used for free, but if you must 
use MP3 in your product, you will have to contact Thompson Multimedia 
and arrange to pay for an MP3 license.  Game developers can get a 
'game license' to use MP3 for $2500.  See 
http://www.mp3licensing.com/royalty/games.html.

Macromedia Flash
The Framework supports Flash through FlashWidget, but you must sign 
up for a license to redistribute the Flash ActiveX control if you 
use FlashWidget in your application.  You can get a Flash license 
from Macromedia at 
http://www.macromedia.com/support/shockwave/info/licensing/main_2.html.
Make sure you actually read the license, particularly the 
"Consideration" section.

J2K-Codec Library
The framework supports loading j2k images through the j2k-codec library.
This library is NOT free and you must purchase your own copy if you want
to use it in registered (non-demo) mode. You may obtain a copy from 
Alex Saveliev at http://j2k-codec.com. We do not distribute our key,
so the library will run in demo mode with the following restrictions,
as taken from the readme.txt file:
  * beeps at the end of each decoding 
  * open the ordering page once in about 30 decoded images 
  * on the rare occasions do several other little nags

The price of the j2k codec is between $49 and $399, depending on use.
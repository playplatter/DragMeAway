all:
	@if [ ! -d ${ANDROID_HOME} ]; then echo "The ANDROID_HOME environment variable is missing. Please add it to your .profile."; exit; fi
	./build_native.py
	ant -Dsdk.dir=/Users/syedasaraahmed/Development/Android/adt-bundle-mac-x86_64-20140321/sdk debug
run:
	adb -d uninstall com.PlayPlatter.DragMeAway
	adb -d install bin/DragMeAway-debug.apk
	adb -d logcat | grep cocos2d
clean:
	rm -rf obj
	rm -rf bin
	rm -rf gen
	rm -rf assets
	rm -rf libs/armeabi
	rm -rf libs/armeabi-v7a
	rm -rf libs/x86
	rm -rf libs/mips
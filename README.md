# GfCompress

How to
To get a Git project into your build:

Step 1. Add the JitPack repository to your build file

gradle
maven
sbt
leiningen
Add it in your root build.gradle at the end of repositories:

	allprojects {
		repositories {
			...
			maven { url 'https://jitpack.io' }
		}
	}
Step 2. Add the dependency

	dependencies {
	        implementation 'com.github.gaof5:GfCompress:v1.0.1'
	}


# 使用说明

 1. 图片压缩前自行配置读写权限

 2. 进行压缩
 ```
	GfCompress.with(this)
		.load(file)//待压缩图片
		.ignoreBy(100)//100k内不进行压缩
		.setTargetDir(getPath())//压缩后的文件路径
		.filter(new CompressionPredicate() {//过滤条件
		    @Override
		    public boolean apply(String path) {
			return !(TextUtils.isEmpty(path) || path.toLowerCase().endsWith(".gif"));
		    }
		})
		.setCompressListener(new OnCompressListener() {//压缩监听
		    @Override
		    public void onStart() { }

		    @Override
		    public void onSuccess(File file) {
			Log.e(TAG, file.getAbsolutePath());
		    }

		    @Override
		    public void onError(Throwable e) { }
		}).launch();
```

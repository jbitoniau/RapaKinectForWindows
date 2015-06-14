# RapaKinectForWindows

Download [Kinect for Windows SDK v1.8](https://www.microsoft.com/en-us/download/details.aspx?id=40278) and install it. Then, in the project main folder:
```
mkdir _build_
cd _build_
cmake .. -G"Visual Studio 10 2010" -DCMAKE_INSTALL_PREFIX="..\_output_
```

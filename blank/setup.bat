mkdir deps
cd deps
git clone https://github.com/redhatdragon/EchoEngine EchoEngine
cd EchoEngine
cmd /c setup.bat
cd ../
cd ../

mkdir ExampleSystems
git clone https://github.com/redhatdragon/EchoEngineECSExampleSystems ExampleSystems
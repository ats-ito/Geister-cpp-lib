# geister-cpp-lib
ガイスターAI開発用の関連ファイル群です

## 使用手順
### ビルドについて
プログラムのビルドには，CMake＆ninjaを使用しています．  
また，コンパイラはclangを想定しています．gccでも動作するかもしれませんが，保証はできません．  
ビルドに関しては，rootにおいてある`build.sh or build.bat`を使用することを推奨します．
### 各ツールについて
- competition  
  自動対戦用ツールです．  
  基本的には，
  ```
  competition {プレイヤ1パス} {プレイヤ2パス}
  ```
  と実行することで，プレイヤ1とプレイヤ2の対戦が実行されます．
  ```
  competition -h
  ```
  と実行すると，オプションが確認できます．
- client  
  https://github.com/miyo/geister_server.java  
  こちらのサーバに接続するためのツールです．  
  基本的には，
  ```
  client -p {ポート番号} {プレイヤパス}
  ```
  として使用します．
  こちらについても，  
  ```
  client -h
  ```
  と実行することでオプションが確認できます．

## プレイヤの開発について
competitionやclientから呼び出せるようにするために，プレイヤは動的ライブラリにしています．  
基本的にはPlayerクラスを継承して，decideHandとdecideRedをオーバライドすればOKです．  
現在，サンプルプレイヤとして，以下の2つが用意してあります．

 名前 | 特徴
 --- | ---
 randomPlayer | 可能な手からランダムに手を決定します．
 chototsPlayer | 猪突戦法と呼ばれる戦法に従います．
  


## 使用ライブラリ
このプログラムでは，以下のライブラリを使用しています．

ライブラリ名 | 用途 | リンク
--- | --- | ---
clxライブラリ | TCP通信 | http://clx.cielquis.net/
nonsugarライブラリ | コマンドパーサ | https://github.com/iorate/nonsugar


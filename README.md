# geister-cpp-lib
ガイスターAI開発用の関連ファイル群です

## 使用方法

### プレイヤの開発手順
#### 簡易版
1. Player/内でrandomPlayer.hppをコピーし，ファイル名を適当に変更．
2. ファイル内のクラス名も適当に変更．
3. 以下の2つの関数の処理を実装
   - std::string decideRed();
   - std::string decideHand(std::string res);
4. ビルド

詳細は[こちら](doc/guide.md)

### ビルド
makefileを用意してあるので，こちらを使用してください．  
基本コマンド
```
make -j
```
ビルド時に次の変数が指定可能です．（括弧内はエイリアス）
- PLAYER_NAME（PN）：   プレイヤのファイル名指定
- PLAYER_CLASS（PC）：  Player.cppで参照するクラス名

例
```
make -j PN=random PC=RandomPlayer
```

### 生成ファイルについて
ビルドが完了すると，binディレクトリが作成され，  
変数の指定がなければ以下のファイルが生成されます．  
（環境によって生成ファイルの拡張子が異なるため，適宜読み替えてください．）

ファイル名 | ファイルタイプ | 説明
--- | --- | ---
competition.exe | 実行ファイル | 対戦実行用プログラム
client.exe | 実行ファイル | サーバ接続用プログラム
randomPlayer.dll | 共有ライブラリ | ランダムプレイヤ
chototsuPlayer.dll | 共有ライブラリ | 猪突戦法プレイヤ
Player.dll | 共有ライブラリ | デフォルトプレイヤ（中身はrandomPlayerと同一）
Player.exe | 実行ファイル | デフォルトプレイヤの実行ファイル版

## 各ツールについて
詳細は[こちら](doc/usage.md)
- competition  
  自動対戦用ツールです．  
  ```
  competition {プレイヤ1パス} {プレイヤ2パス}
  ```
  と実行することで，プレイヤ1とプレイヤ2の対戦が実行されます．
  
- client  
  https://github.com/miyo/geister_server.java  
  こちらのサーバに接続するためのツールです．  
  ```
  client -p {ポート番号} {プレイヤパス}
  ```
  のように使用します．

## ファイル構成について
詳細は[こちら](doc/spec.md)

## 動作確認済み環境
- MinGW64 & clang++(8.0.0)
- MinGW64 & g++(9.1.0)
- WSL & clang++(7.1.0)
- WSL & g++(7.4.0)
- MSVC & clang++(8.0.0)
- CentOS & g++(8.2.0)

Macについては検証環境がないため未確認となります

## 使用ライブラリ
このプログラムでは，以下のライブラリを使用しています．

ライブラリ名 | 用途 | リンク
--- | --- | ---
clxライブラリ | TCP通信 | http://clx.cielquis.net/
nonsugarライブラリ | コマンドラインパーサ | https://github.com/iorate/nonsugar


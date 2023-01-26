# コーディングスタイル
## 命名規則
### エイリアス
#### using
+ shared_ptr\<Hoge\>
接尾辞にChildRef。
Use suffix ChildRef
```c++
using HogeChildRef=std::shared_ptr<Hoge>;
```

+ weak_ptr\<Hoge\>
接尾辞にParentRef。
Use suffix ParentRef.
```c++
using HogeParentRef=std::weak_ptr<Hoge>;
```

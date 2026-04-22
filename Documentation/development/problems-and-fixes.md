# Problems and Fixes

*Things that didn't work and how they are solved*

## Model Layer

### `script`
`elements_` was of type QList. QList uses implicit sharing (copy-on-write) internally which requires 
elements to be copyable. As `unique_ptr` is not copyable, `QList` and `unique_ptr` are not 
compatible. Due to this `elements_` was changed to `std::vector`. This also has the benefit of the
model layer being in pure C++ and not reliant on the Qt framework.
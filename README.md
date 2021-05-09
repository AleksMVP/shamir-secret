# Shamir secret sharing

## Docker build
```
git clone https://github.com/AleksMVP/shamir-secret.git
cd shamir-secret
docker build -t superbober .
docker run -it superbober
```

## Usage 

### Split mode
```
> ./main.out split
N M secret
```
* N - shares count
* M - threshold entry
* secret - secret key in hex format

### Recover mode
```
> ./main.out recover
M
X1 Y1
X2 Y2
```

* M - points count
* X1, Y1 - point coords


## Example

```
> ./main.out split
3 2 123456

1 d80f0500c634055569fab9db6eea03298ccb9da93220c862bfdde2551736505e
2 b01e0a018c680aaad3f573b6ddd4065319973b52644190c57fbbc4aa2e5a6d23
3 882d0f02529c10003df02d924cbe097ca662d8fb966259283f99a6ff457e89e8

> ./main.out recover
2
2 b01e0a018c680aaad3f573b6ddd4065319973b52644190c57fbbc4aa2e5a6d23
3 882d0f02529c10003df02d924cbe097ca662d8fb966259283f99a6ff457e89e8

0000000000000000000000000000000000000000000000000000000000123456
```



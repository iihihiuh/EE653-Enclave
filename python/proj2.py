import torch

from python.sgxutils import SGXUtils

def main(args):
    sgxutils = SGXUtils()

    l = torch.nn.Linear(args.in_features, args.out_features, bias=False).cuda()
    x = torch.randn(args.batch, args.in_features).cuda()


    # given the weight; precompute w * r
    sgxutils.precompute(l.weight, args.batch)

    # x_blinded = x + r
    x_blinded = sgxutils.addNoise(x)

    # y_blinded = w * x_blinded
    y_blinded = l(x_blinded)

    # y_recovered = y_blinded - w * r
    y_recovered = sgxutils.removeNoise(y_blinded)
    s = sgxutils.nativeMatMul(l.weight, x)

    y_expected = l(x)

    print("Total diffs:", abs(y_expected - y_recovered).sum())

    return 0
    

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--in_features', type=int, default=10, help="Input feature of Linear layer")
    parser.add_argument('--out_features', type=int, default=30,  help="Output feature of Linear layer")
    parser.add_argument('--batch', type=int, default=30, help="Input batch size")

    args = parser.parse_args()
    main(args)